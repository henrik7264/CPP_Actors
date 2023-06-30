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
#include "Message.h"
#include "Executor.h"
#include "Scheduler.h"

using namespace Messages;


namespace StateMachines
{
    const static long UNDEFINED_STATE = -1;
    class StateMachine;

    struct LongWrapper {
        long value;
        explicit LongWrapper(long value): value(value) {}
        operator long() const {return value;}
    };
    typedef LongWrapper Timeout;
    typedef LongWrapper StateId;
    typedef LongWrapper NextState;
    typedef LongWrapper Initial_State;


    class SMDispatcher
    {
    private:
        std::mutex smdMutex;
        std::map<MessageType, std::list<StateMachine*>> stateMachinesMap;

        SMDispatcher() = default;
        virtual ~SMDispatcher() = default;

    public:
        static SMDispatcher& getInstance() {
            static SMDispatcher MySMDispatcher;
            return MySMDispatcher;
        }

        void unregisterSM(StateMachine* stateMachine);
        void registerSM(StateMachine* stateMachine);
        void publish(const Message_ptr& message);
    }; // SMDispatcher


    enum VarArgType {STATE_VA, TIMER_VA, MESSAGE_VA};
    class VarArg
    {
    private:
        VarArgType varArgType;

    public:
        explicit VarArg(VarArgType type): varArgType(type) {}
        virtual ~VarArg() = default;

        VarArgType getVarArcType() const {return varArgType;}
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


    class MessageTrans: public Transition
    {
    private:
        MessageType msgType;
        std::function<void(Message_ptr)> action;

    public:
        MessageTrans(MessageType msgType, const NextState& nextState): Transition(VarArgType::MESSAGE_VA, nextState), msgType(msgType), action([](const Message_ptr& msg){/* do nothing */}) {}
        MessageTrans(MessageType msgType, const std::function<void(Message_ptr)>& func): Transition(VarArgType::MESSAGE_VA, NextState(UNDEFINED_STATE)), msgType(msgType), action(func) {}
        MessageTrans(MessageType msgType, const NextState& nextState, const std::function<void(Message_ptr)>& func): Transition(VarArgType::MESSAGE_VA, nextState), msgType(msgType), action(func) {}
        ~MessageTrans() override = default;

        MessageType getMsgType() const {return msgType;}
        void update(const Message_ptr& msg);
    }; // Trans_Message


    class TimerTrans: public  Transition
    {
    private:
        Timeout timeout;
        std::function<void()> action;

    public:
        TimerTrans(const Timeout& timeout, const NextState& nextState): Transition(VarArgType::TIMER_VA, nextState), timeout(timeout), action([](){/* do nothing */}) {}
        TimerTrans(const Timeout& timeout, const std::function<void()>& func): Transition(VarArgType::TIMER_VA, NextState(UNDEFINED_STATE)), timeout(timeout), action(func) {}
        TimerTrans(const Timeout& timeout, const NextState& nextState, const std::function<void()>& func): Transition(VarArgType::TIMER_VA, nextState), timeout(timeout), action(func) {}
        TimerTrans(long timeout, const NextState& nextState): Transition(VarArgType::TIMER_VA, nextState), timeout(timeout), action([](){/* do nothing */}) {}
        TimerTrans(long timeout, const std::function<void()>& func): Transition(VarArgType::TIMER_VA, NextState(UNDEFINED_STATE)), timeout(timeout), action(func) {}
        TimerTrans(long timeout, const NextState& nextState, const std::function<void()>& func): Transition(VarArgType::TIMER_VA, nextState), timeout(timeout), action(func) {}
        ~TimerTrans() override = default;

        Timeout getTimeout() const {return timeout;}
        void update();
    }; // Transition_Timeout


    class State: public VarArg
    {
    private:
        StateId stateId;
        std::list<VarArg*> args; // List of events

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
            for(auto arg: args) {
                auto trans = dynamic_cast<Transition*>(arg);
                trans->setStateMachine(stateMachine);
            }
        }

        StateId getStateId() const {return stateId;}

        const std::list<VarArg*>& getArgs() const {return args;}

        void update(const Message_ptr& msg) {
            for (auto arg: args) {
                if (arg->getVarArcType() == VarArgType::MESSAGE_VA) {
                    auto message = dynamic_cast<MessageTrans*>(arg);
                    if (message->getMsgType() == msg->getMsgType())
                        message->update(msg);
                }
            }
        }
    }; // State


    class StateMachine
    {
    private:
        std::mutex smMutex;
        std::mutex& actorMutex;
        StateId currState;
        std::list<VarArg*> args;
        std::list<Schedulers::JobId> jobIds;

    public:
        template<typename ... States>
        explicit StateMachine(std::mutex& mutex, const Initial_State& initialState, States... states) : actorMutex(mutex), currState(initialState), args({states...}) {
            jobIds.clear();
            for (auto arg: args) {
                assert(arg->getVarArcType() == VarArgType::STATE_VA);
                auto state = dynamic_cast<State*>(arg);
                state->setStateMachine(this);
            }
            setCurrState(initialState);
            SMDispatcher::getInstance().registerSM(this);
        }

        template<typename ... States>
        explicit StateMachine(std::mutex& mutex, long initialState, States... states): StateMachine(mutex, Initial_State(initialState), states...) {}

        virtual ~StateMachine() {
            std::unique_lock<std::mutex> lock(smMutex);
            SMDispatcher::getInstance().unregisterSM(this);
            for (auto jobId: jobIds)
                Schedulers::Scheduler::getInstance().removeJob(jobId);
            for (auto arg: args)
                delete arg;
        }

        std::mutex& getSMMutex() {return smMutex;}
        std::mutex& getActorMutex() {return actorMutex;}

        const std::list<VarArg*>& getArgs() const {return args;}

        StateId getCurrState() const {return currState;}

        void setCurrState(const StateId& currentState) {
            //std::unique_lock<std::mutex> lock(smMutex); // not needed is indirectly locked by the update.
            for (auto jobId: jobIds)
                Schedulers::Scheduler::getInstance().removeJob(jobId);
            jobIds.clear();
            currState = currentState;
            for (auto arg: args) {
                auto state = dynamic_cast<State*>(arg);
                if (state->getStateId() == currState) {
                    for (auto trans: state->getArgs()) {
                        if (trans->getVarArcType() == VarArgType::TIMER_VA) {
                            auto timer = dynamic_cast<TimerTrans*>(trans);
                            jobIds.push_back(Schedulers::Scheduler::getInstance().onceIn(timer->getTimeout(),[timer]() { timer->update(); }));
                        }
                    }
                }
            }
        }

        void update(const Message_ptr& msg) {
            if (smMutex.try_lock()) {
                for (auto arg: args) {
                    auto state = dynamic_cast<State *>(arg);
                    if (currState == state->getStateId())
                        state->update(msg);
                }
                smMutex.unlock();
            }
        }
    }; // StateMachine


    void SMDispatcher::unregisterSM(StateMachine* stateMachine) {
        std::unique_lock<std::mutex> lock(smdMutex);
        for (auto& elem : stateMachinesMap) {
            elem.second.remove_if([stateMachine](StateMachine* sm){ return sm == stateMachine;});
            stateMachinesMap[elem.first] = elem.second;
        }
    }

    void SMDispatcher::registerSM(StateMachine* stateMachine) {
        std::unique_lock<std::mutex> lock(smdMutex);
        if (stateMachine != nullptr) {
            for (auto state_arg: stateMachine->getArgs()) {
                auto state = dynamic_cast<State*>(state_arg);
                for (auto trans: state->getArgs()) {
                    if (trans->getVarArcType() == VarArgType::MESSAGE_VA) {
                        auto message = dynamic_cast<MessageTrans*>(trans);
                        std::list<StateMachine*> stateMachineList;
                        if (stateMachinesMap.find(message->getMsgType()) != stateMachinesMap.end())
                            stateMachineList = stateMachinesMap[message->getMsgType()];
                        stateMachineList.push_back(stateMachine);
                        stateMachinesMap[message->getMsgType()] = stateMachineList;
                    }
                }
            }
        }
    }

    void SMDispatcher::publish(const Message_ptr& message) {
        std::unique_lock<std::mutex> lock(smdMutex);
        if (stateMachinesMap.find(message->getMsgType()) != stateMachinesMap.end())
            for (auto stateMachine: stateMachinesMap[message->getMsgType()])
                Executors::Executor::getInstance().exec([stateMachine](const Message_ptr msg){stateMachine->update(msg);}, message);
    }

    void MessageTrans::update(const Message_ptr& msg) {
        std::unique_lock<std::mutex> lock(stateMachine->getActorMutex());
        action(msg);
        if (nextState != UNDEFINED_STATE)
            stateMachine->setCurrState(nextState);
    }

    void TimerTrans::update() {
        if (stateMachine->getSMMutex().try_lock()) {
            std::unique_lock<std::mutex> lock(stateMachine->getActorMutex());
            action();
            if (nextState != UNDEFINED_STATE)
                stateMachine->setCurrState(nextState);
            stateMachine->getSMMutex().unlock();
        }
    }
} // StateMachines

#endif //CPP_ACTORS_STATEMACHINE_H
