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

#ifndef CPP_ACTORS_DISPATCHER_H
#define CPP_ACTORS_DISPATCHER_H
#include <cassert>
#include <functional>
#include <memory>
#include <list>
#include <thread>
#include <mutex>
#include "Queue.h"
#include "Message.h"
#include "Executor.h"

using namespace Messages;


namespace Dispatchers
{
    class Dispatcher
    {
    private:
        std::mutex mutex;
        std::list<std::function<void(Message_ptr)>> funcLists[MessageType::NO_OF_MSG_TYPES];

        Dispatcher() = default;
        virtual ~Dispatcher() {
            std::unique_lock<std::mutex> lock(mutex);
            for (auto & funcList : funcLists)
                funcList.clear();
        };

    public:
        static Dispatcher& getInstance() {
            static Dispatcher MyDispatcher;
            return MyDispatcher;
        }

        void subscribe(MessageType type, const std::function<void(Message_ptr)>& func) {
            assert(type != MessageType::NONE && type != MessageType::NO_OF_MSG_TYPES);
            std::unique_lock<std::mutex> lock(mutex);
            funcLists[type].push_back(func);
        }

        void unsubscribe(MessageType type, const std::function<void(Message_ptr)>& func) {
            assert(type != MessageType::NONE && type != MessageType::NO_OF_MSG_TYPES);
            std::unique_lock<std::mutex> lock(mutex);
            funcLists[type].remove_if([func](const std::function<void(Message_ptr)>& f){return f.target<std::function<void(Message_ptr)>>() == func.target<std::function<void(Message_ptr)>>();});
        }

        void publish(const Message_ptr& msg) {
            std::unique_lock<std::mutex> lock(mutex);
            for (const auto& func: funcLists[msg->getMsgType()])
                Executors::Executor::getInstance().exec(func, msg);
        }
    }; // Dispatcher
} // Messages

#endif //CPP_ACTORS_DISPATCHER_H
