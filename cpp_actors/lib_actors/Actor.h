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

#ifndef CPP_ACTORS_ACTOR_H
#define CPP_ACTORS_ACTOR_H
#include <chrono>
#include <mutex>
#include <functional>
#include <string>
#include <rxcpp/rx.hpp>
#include "Memory.h"
#include "Logger.h"
#include "Message.h"
#include "Dispatcher.h"
#include "Scheduler.h"
#include "Timer.h"
#include "StateMachine.h"

#define STATEMACHINE(...) StateMachine_t(new StateMachines::StateMachine(Actor::actorMutex, __VA_ARGS__))
#define STATE(...) new StateMachines::State(__VA_ARGS__)
#define TIMER(...) new StateMachines::TimerTransition(__VA_ARGS__)
#define MESSAGE(...) new StateMachines::MessageTransition(__VA_ARGS__)
#define NEXT_STATE(nextState) StateMachines::NextState(nextState)

using namespace Messages;


namespace Actors
{
    typedef Dispatchers::Function_t DispatcherFunction_t;
    typedef Dispatchers::FuncId_t SubscriptionId_t;
    typedef Schedulers::Function_t SchedulerFunction_t;
    typedef Schedulers::JobId_t JobId_t;
    typedef std::shared_ptr<Timers::Timer> Timer_t;
    typedef std::shared_ptr<StateMachines::StateMachine> StateMachine_t;

    class Messenger
    {
    private:
        bool& markedForDeletion;
        std::mutex& actorMutex;

    protected:
        std::mutex subscriptionsMutex;
        std::map<SubscriptionId_t, Message_t> subscriptions;

    public:
        explicit Messenger(bool& markedForDeletion, std::mutex& actorMutex): markedForDeletion(markedForDeletion), actorMutex(actorMutex) {};
        virtual ~Messenger() = default;

        SubscriptionId_t subscribe(Message_t type, const DispatcherFunction_t& func) {
            std::unique_lock<std::mutex> lock(subscriptionsMutex);
            for (const auto& sub: subscriptions)
                assert(sub.second != type);
            auto fn = [this, func](Message* msg){
                std::unique_lock<std::mutex> lock(actorMutex);
                if (!markedForDeletion)
                    func(msg);};
            auto subId = Dispatchers::Dispatcher::getInstance().registerCB(fn, type);
            subscriptions[subId] = type;
            return subId;
        }

        void unsubscribe(const SubscriptionId_t subId) {
            std::unique_lock<std::mutex> lock(subscriptionsMutex);
            auto type = subscriptions[subId];
            Dispatchers::Dispatcher::getInstance().unregisterCB(subId, type);
            subscriptions.erase(subId);
        }

        static void publish(Message* msg) {
            Dispatchers::Dispatcher::getInstance().publish(msg);
        }

        auto stream(Message_t type) {
            auto observable = rxcpp::observable<>::create<Message*> (
                [=](const rxcpp::subscriber<Message*>& subscriber) {
                    subscribe(type, [=](Message* msg) {subscriber.on_next(msg);});
                });
            return observable;
        }
    }; // Messenger


    class Scheduler
    {
    private:
        bool& markedForDeletion;
        std::mutex& actorMutex;

    protected:
        std::mutex scheduledJobsMutex;
        std::list<JobId_t> scheduledJobs;

    public:
        explicit Scheduler(bool& markedForDeletion, std::mutex& actorMutex): markedForDeletion(markedForDeletion), actorMutex(actorMutex) {};
        virtual ~Scheduler() = default;

        JobId_t once(std::chrono::duration<long, std::milli> msec, const SchedulerFunction_t& func) {
            std::unique_lock<std::mutex> lock(scheduledJobsMutex);
            auto jobId = Schedulers::Scheduler::getInstance().onceIn(msec, [this, func](){
                std::unique_lock<std::mutex> lock(actorMutex);
                if (!markedForDeletion)
                    func();});
            scheduledJobs.push_back(jobId); // Used by destructor to remove subscriptions
            return jobId;
        }

        JobId_t once(long msec, const SchedulerFunction_t& func) {
            return once(std::chrono::duration<long, std::milli>(msec), func);
        }

        JobId_t repeat(std::chrono::duration<long, std::milli> msec, const SchedulerFunction_t& func) {
            std::unique_lock<std::mutex> lock(scheduledJobsMutex);
            auto jobId = Schedulers::Scheduler::getInstance().repeatEvery(msec, [this, func](){
                std::unique_lock<std::mutex> lock(actorMutex);
                if (!markedForDeletion)
                    func();});
            scheduledJobs.push_back(jobId); // Used by destructor to remove subscriptions
            return jobId;
        }

        JobId_t repeat(long msec, const SchedulerFunction_t& func) {
            return repeat(std::chrono::duration<long, std::milli>(msec), func);
        }

        void remove(JobId_t jobId) {
            std::unique_lock<std::mutex> lock(scheduledJobsMutex);
            Schedulers::Scheduler::getInstance().removeJob(jobId);
            scheduledJobs.remove(jobId); // Used by destructor to remove subscriptions
        }

        Timer_t timer(std::chrono::duration<long, std::milli> msec, const SchedulerFunction_t& func) {
            return Timer_t(new Timers::Timer(actorMutex, msec, func));
        }

        Timer_t timer(long msec, const SchedulerFunction_t& func) {
            return Timer_t (new Timers::Timer(actorMutex, msec, func));
        }
    }; // Scheduler

    class Logger
    {
    protected:
        std::string name;

    public:
        explicit Logger(std::string  name): name(std::move(name)) {};
        virtual ~Logger() = default;

        inline Loggers::Logger debug() {return Loggers::Logger::debug(name);}
        inline Loggers::Logger info() {return Loggers::Logger::info(name);}
        inline Loggers::Logger warning() {return Loggers::Logger::warning(name);}
        inline Loggers::Logger error() {return Loggers::Logger::error(name);}
        inline Loggers::Logger fatal() {return Loggers::Logger::fatal(name);}
    }; // Logger


    class Actor: public MemoryManagement::Memory, public Messenger, public Scheduler, public Logger
    {
    protected:
        bool markedForDeletion = false;
        std::mutex actorMutex;
        std::string actorName;

    public:
        explicit Actor(const std::string& name): Messenger(markedForDeletion, actorMutex), Scheduler(markedForDeletion,actorMutex), Logger(name), markedForDeletion(false), actorName(name) {}

        ~Actor() override {
            markedForDeletion = true;

            std::unique_lock<std::mutex> scheduledJobsLock(scheduledJobsMutex);
            for (const auto jobId: scheduledJobs)
                Schedulers::Scheduler::getInstance().removeJob(jobId);
            scheduledJobs.clear();

            std::unique_lock<std::mutex> subscriptionsLock(subscriptionsMutex);
            for (const auto& sub: subscriptions)
                Dispatchers::Dispatcher::getInstance().unregisterCB(sub.first, sub.second);
            subscriptions.clear();
        }
    }; // Actor
} // Actors

#endif //CPP_ACTORS_ACTOR_H
