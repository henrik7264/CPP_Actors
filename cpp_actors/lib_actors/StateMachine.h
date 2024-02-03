/*
 * Copyright (c) 2023, Henrik Larsen
 * https://github.com/henrik7264/CPP_Actors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CPP_ACTORS_STATEMACHINE_H
#define CPP_ACTORS_STATEMACHINE_H
#include <cassert>
#include <memory>
#include <functional>
#include <mutex>
#include <list>
#include <map>
#include <utility>
#include "Memory.h"
#include "Message.h"
#include "Dispatcher.h"
#include "Scheduler.h"

using namespace Messages;


namespace StateMachines
{
    typedef Dispatchers::Function_t DispatcherFunction_t;
    typedef Dispatchers::FuncId_t SubscriptionId_t;
    typedef Schedulers::Function_t SchedulerFunction_t;
    typedef Schedulers::JobId_t JobId_t;
    const static long UNDEFINED_STATE = -1;

    struct LongWrapper {
        long value;
        explicit LongWrapper(long value): value(value) {}
        operator long() const {return value;}
    };
    typedef LongWrapper Timeout;
    typedef LongWrapper StateId;
    typedef LongWrapper NextState;
    typedef LongWrapper Initial_State;

    class StateMachine;


    enum VarArgType {STATE_VA, TIMER_VA, MESSAGE_VA};
    class VarArg
    {
    private:
        VarArgType varArgType;

    public:
        explicit VarArg(VarArgType type): varArgType(type) {}
        virtual ~VarArg() = default;

        inline VarArgType getVarArcType() const {return varArgType;}
    };


    class Transition: public VarArg
    {
    protected:
        StateMachine* stateMachine;
        NextState nextState;

    public:
        explicit Transition(VarArgType type, const NextState& nextState): VarArg(type), stateMachine(nullptr), nextState(nextState) {}
        ~Transition() override = default;

        void setStateMachine(StateMachine* sm) {stateMachine = sm;}
    }; // Transition


    class MessageTransition: public MemoryManagement::Memory, public Transition
    {
    private:
        Message_t msgType;
        DispatcherFunction_t action;

    public:
        MessageTransition(Message_t msgType, NextState nextState): Transition(VarArgType::MESSAGE_VA, nextState), msgType(msgType), action([](const Message_ptr& msg){/* do nothing */}) {}
        MessageTransition(Message_t msgType, DispatcherFunction_t func): Transition(VarArgType::MESSAGE_VA, NextState(UNDEFINED_STATE)), msgType(msgType), action(std::move(func)) {}
        MessageTransition(Message_t msgType, NextState nextState, DispatcherFunction_t func): Transition(VarArgType::MESSAGE_VA, nextState), msgType(msgType), action(std::move(func)) {}
        ~MessageTransition() override = default;

        inline Message_t getMsgType() const {return msgType;}
        void doAction(const Message_ptr& msg);
    }; // MessageTransition


    class TimerTransition: public MemoryManagement::Memory, public  Transition
    {
    private:
        Timeout timeout;
        SchedulerFunction_t action;

    public:
        TimerTransition(const Timeout& timeout, NextState nextState): Transition(VarArgType::TIMER_VA, nextState), timeout(timeout), action([](){/* do nothing */}) {}
        TimerTransition(const Timeout& timeout, SchedulerFunction_t  func): Transition(VarArgType::TIMER_VA, NextState(UNDEFINED_STATE)), timeout(timeout), action(std::move(func)) {}
        TimerTransition(const Timeout& timeout, NextState nextState, SchedulerFunction_t  func): Transition(VarArgType::TIMER_VA, nextState), timeout(timeout), action(std::move(func)) {}
        TimerTransition(long timeout, NextState nextState): Transition(VarArgType::TIMER_VA, nextState), timeout(timeout), action([](){/* do nothing */}) {}
        TimerTransition(long timeout, SchedulerFunction_t  func): Transition(VarArgType::TIMER_VA, NextState(UNDEFINED_STATE)), timeout(timeout), action(std::move(func)) {}
        TimerTransition(long timeout, NextState nextState, SchedulerFunction_t  func): Transition(VarArgType::TIMER_VA, nextState), timeout(timeout), action(std::move(func)) {}
        ~TimerTransition() override = default;

        inline Timeout getTimeout() const {return timeout;}
        void doAction();
    }; // TimerTransition


    class State: public MemoryManagement::Memory, public VarArg
    {
    private:
        StateId stateId;
        std::list<VarArg*> args; // List of transitions

    public:
        template<typename ... Transitions>
        explicit State(const StateId& stateId, Transitions... transitions) : VarArg(VarArgType::STATE_VA), stateId(stateId), args({transitions...}) {
            for(auto arg: args)
                assert(arg->getVarArcType() == VarArgType::TIMER_VA || arg->getVarArcType() == VarArgType::MESSAGE_VA);
        }

        template<typename ... Transitions>
        explicit State(long stateId, Transitions... transitions): State(StateId(stateId), transitions...) {}

        ~State() override {
            for (auto arg: args)
                delete arg;
        }

        void setStateMachine(StateMachine* stateMachine) {
            for (auto arg: args) {
                auto trans = dynamic_cast<Transition*>(arg);
                trans->setStateMachine(stateMachine);
            }
        }

        inline StateId getStateId() const {return stateId;}
        inline const std::list<VarArg*>& getTransitions() const {return args;}
    }; // State


    class StateMachine: public MemoryManagement::Memory
    {
    private:
        bool markedForDeletion;
        std::mutex& actorMutex;
        std::mutex mutex;
        StateId currState;
        std::list<VarArg*> args; // list of states
        std::list<JobId_t> jobs;
        std::list<std::pair<SubscriptionId_t, Message_t>> subscriptions;

    public:
        template<typename ... States>
        explicit StateMachine(std::mutex& actorMutex, const Initial_State& initialState,  States... states) : markedForDeletion(false), actorMutex(actorMutex), currState(initialState), args({states...}) {
            jobs.clear();
            subscriptions.clear();
            for (auto arg: args) {
                assert(arg->getVarArcType() == VarArgType::STATE_VA);
                auto* state = dynamic_cast<State*>(arg);
                state->setStateMachine(this);
            }
            setCurrState(initialState);
        }

        template<typename ... States>
        explicit StateMachine(std::mutex& actorMutex, long initialState, States... states): StateMachine(actorMutex, Initial_State(initialState), states...) {}

        virtual ~StateMachine() {
            std::unique_lock<std::mutex> lock(mutex);
            markedForDeletion = true;
            for (auto job: jobs)
                Schedulers::Scheduler::getInstance().removeJob(job);
            jobs.clear();
            for (auto subs: subscriptions)
                Dispatchers::Dispatcher::getInstance().unregisterCB(subs.first, subs.second);
            subscriptions.clear();
            for (auto arg: args)
                delete arg;
        }

        inline bool getMarkedForDeletion() const {return markedForDeletion;}
        inline std::mutex& getActorMutex() {return actorMutex;}

        void setCurrState(const StateId& currentState) {
            std::unique_lock<std::mutex> lock(mutex);
            if (!markedForDeletion) {
                for (auto job: jobs)
                    Schedulers::Scheduler::getInstance().removeJob(job);
                jobs.clear();
                for (auto subs: subscriptions)
                    Dispatchers::Dispatcher::getInstance().unregisterCB(subs.first, subs.second);
                subscriptions.clear();

                currState = currentState;

                for (auto arg: args) {
                    auto* state = dynamic_cast<State*>(arg);
                    if (state->getStateId() == currState) {
                        for (auto* trans: state->getTransitions()) {
                            if (trans->getVarArcType() == VarArgType::TIMER_VA) {
                                auto* timerTrans = dynamic_cast<TimerTransition*>(trans);
                                jobs.push_back(Schedulers::Scheduler::getInstance().onceIn(timerTrans->getTimeout(), [timerTrans] () {timerTrans->doAction();}));
                            }
                            if (trans->getVarArcType() == VarArgType::MESSAGE_VA) {
                                auto* msgTrans = dynamic_cast<MessageTransition*>(trans);
                                auto msgType = msgTrans->getMsgType();
                                subscriptions.emplace_back(Dispatchers::Dispatcher::getInstance().registerCB([msgTrans](const Message_ptr& msg) {msgTrans->doAction(msg);}, msgType), msgType);
                            }
                        }
                    }
                }
            }
        }
    }; // StateMachine

    void MessageTransition::doAction(const Message_ptr& msg) {
        if (!stateMachine->getMarkedForDeletion()) {
            {std::unique_lock<std::mutex> lock(stateMachine->getActorMutex());
            action(msg);}
            if (nextState!=UNDEFINED_STATE)
                stateMachine->setCurrState(nextState);
        }
    }

    void TimerTransition::doAction() {
        if (!stateMachine->getMarkedForDeletion()) {
            {std::unique_lock<std::mutex> lock(stateMachine->getActorMutex());
            action();}
            if (nextState!=UNDEFINED_STATE)
                stateMachine->setCurrState(nextState);
        }
    }
} // StateMachines

#endif //CPP_ACTORS_STATEMACHINE_H
