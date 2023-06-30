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
#include <cassert>
#include <chrono>
#include <memory>
#include <mutex>
#include <functional>
#include <string>
#include <list>
#include <tuple>
#include <rxcpp/rx.hpp>
#include <utility>
#include "Logger.h"
#include "Executor.h"
#include "Message.h"
#include "Dispatcher.h"
#include "Scheduler.h"
#include "Timer.h"
#include "StateMachine.h"

#define STATEMACHINE(...)  std::shared_ptr<StateMachines::StateMachine>(new StateMachines::StateMachine(mutex, __VA_ARGS__))
#define STATE(...) new StateMachines::State(__VA_ARGS__)
#define TIMER(...) new StateMachines::TimerTrans(__VA_ARGS__)
#define MESSAGE(...) new StateMachines::MessageTrans(__VA_ARGS__)
#define NEXT_STATE(nextState) StateMachines::NextState(nextState)


using namespace Messages;


namespace Actors
{
    typedef std::shared_ptr<Timers::Timer> Timer_t;
    typedef std::shared_ptr<StateMachines::StateMachine> StateMachine_t;

    class Message
    {
    private:
        std::list<std::pair<MessageType, std::function<void(Message_ptr)>>> subscriptions;
        std::mutex& mutex;

    public:
        explicit Message(std::mutex& mutex): mutex(mutex) {}
        virtual ~Message() {
            std::unique_lock<std::mutex> lock(mutex);
            for (const auto& subscription: subscriptions)
                Dispatchers::Dispatcher::getInstance().unsubscribe(subscription.first, subscription.second);
        }

        void subscribe(MessageType type, const std::function<void(Message_ptr)>& func) {
            std::unique_lock<std::mutex> lock(mutex);
            for (const auto& subscription: subscriptions)
                assert(subscription.first != type);
            subscriptions.emplace_back(type, func);
            Dispatchers::Dispatcher::getInstance().subscribe(type, [=](const Message_ptr& msg){
                std::unique_lock<std::mutex> lock(mutex);
                func(msg);
            });
        }

        void publish(Messages::Message* msg) {
            auto msg_ptr = Message_ptr(msg);
            Dispatchers::Dispatcher::getInstance().publish(msg_ptr);
            StateMachines::SMDispatcher::getInstance().publish(msg_ptr);
        }

        auto stream(MessageType type) {
            auto observable = rxcpp::observable<>::create<Message_ptr> (
                [=](const rxcpp::subscriber<Message_ptr>& subscriber) {
                    Message::subscribe(type, [=](const Message_ptr& msg) {subscriber.on_next(msg);});
                });
            return observable;
        }
    }; // Message


    class Scheduler
    {
    private:
        std::list<Schedulers::JobId> scheduledJobs;
        std::mutex& mutex;

    public:
        explicit Scheduler(std::mutex& mutex): mutex(mutex) {};
        virtual ~Scheduler() {
            std::unique_lock<std::mutex> lock(mutex);
            for (auto jobId : scheduledJobs)
                Schedulers::Scheduler::getInstance().removeJob(jobId);
        };

        Schedulers::JobId once(std::chrono::duration<long, std::milli> msec, const std::function<void()>& func) {
            std::unique_lock<std::mutex> lock(mutex);
            Schedulers::JobId id = Schedulers::Scheduler::getInstance().onceIn(msec, [this, func](){
                std::unique_lock<std::mutex> lock(mutex);
                func();
            });
            scheduledJobs.push_back(id); // Used by destructor to remove jobs
            return id;
        }

        Schedulers::JobId once(long msec, const std::function<void()>& func) {
            std::unique_lock<std::mutex> lock(mutex);
            Schedulers::JobId id = Schedulers::Scheduler::getInstance().onceIn(msec, [this, func](){
                std::unique_lock<std::mutex> lock(mutex);
                func();
            });
            scheduledJobs.push_back(id); // Used by destructor to remove jobs
            return id;
        }

        Schedulers::JobId repeat(std::chrono::duration<long, std::milli> msec, const std::function<void()>& func) {
            std::unique_lock<std::mutex> lock(mutex);
            Schedulers::JobId id = Schedulers::Scheduler::getInstance().repeatEvery(msec, [this, func](){
                std::unique_lock<std::mutex> lock(mutex);
                func();
            });
            scheduledJobs.push_back(id); // Used by destructor to remove jobs
            return id;
        }

        Schedulers::JobId repeat(long msec, const std::function<void()>& func) {
            std::unique_lock<std::mutex> lock(mutex);
            Schedulers::JobId id = Schedulers::Scheduler::getInstance().repeatEvery(msec, [this, func](){
                std::unique_lock<std::mutex> lock(mutex);
                func();
            });
            scheduledJobs.push_back(id); // Used by destructor to remove jobs
            return id;
        }

        void remove(Schedulers::JobId id) {
            std::unique_lock<std::mutex> lock(mutex);
            Schedulers::Scheduler::getInstance().removeJob(id);
            scheduledJobs.remove(id); // Used by destructor to remove jobs
        }
    }; // Scheduler


    class Timer
    {
    private:
        std::mutex& mutex;

    public:
        explicit Timer(std::mutex& mutex): mutex(mutex) {};
        virtual ~Timer() = default;

        Timer_t getInstance(long msec, const std::function<void()>& func) {
            return std::make_shared<Timers::Timer>(msec, [this, func](){
                std::unique_lock<std::mutex> lock(mutex);
                func();
            });
        }
    }; // Timer


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


    class Actor: public Message, public Scheduler, public Timer, public Logger
    {
    protected:
        std::mutex mutex;

    public:
        explicit Actor(const std::string& name): Message(mutex), Scheduler(mutex), Timer(mutex), Logger(name) {}
        ~Actor() override = default;
    }; // Actor
} // Actors

#endif //CPP_ACTORS_ACTOR_H
