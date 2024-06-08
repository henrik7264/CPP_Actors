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
#include <vector>
#include <list>
#include <thread>
#include <mutex>
#include <fstream>
#include <iterator>
#include "Queue.h"
#include "Message.h"

using namespace Messages;


namespace Dispatchers
{
    typedef std::function<void(Message*)> Function_t;
    typedef unsigned long FuncId_t;
    static FuncId_t NextFuncId = 0;
    static std::vector<Function_t> cbFuncs;
    static std::list<FuncId_t> cbFuncIds[Message_t::NO_OF_MSG_TYPES];
    static std::mutex mutex;

    class Worker
    {
    private:
        bool doLoop = true;
        std::thread trd;
        Queues::Queue<Message*> jobQueue{nullptr};

        void run()
        {
            while (doLoop) {
                auto msg = jobQueue.get(100);
                if (msg) { // msg is null if the queue times out.
                    if (doLoop) {
                        std::unique_lock<std::mutex> lock(mutex);
                        auto funcIds = cbFuncIds[msg->getMsgType()];
                        lock.unlock();
                        for (auto it = funcIds.begin(); it != funcIds.end(); it++) {
                            auto func = cbFuncs[*it];
                            if (doLoop && func != nullptr)
                                func(msg);
                        }
                    }
                    delete msg;
                }
            }
        }

        void stop() {
            doLoop = false;
            if (trd.joinable())
                trd.join();
            while (jobQueue.size() > 0)
                delete jobQueue.get();
        }

    public:
        Worker() { trd = std::thread([this]() { run(); }); }
        virtual ~Worker() { stop(); }

        inline Queues::Queue <Message*>& getQueue() { return jobQueue; }
    }; // Worker


    class Dispatcher
    {
    private:
        std::vector<Worker*> workers;
        std::size_t noWorkers = 0;

        static unsigned int noOfCpus() {
            unsigned int cores = std::thread::hardware_concurrency();
            if (cores == 0)
                cores = 4;
            return cores;
        }

        Dispatcher() {
            for (unsigned int i = 0; i < noOfCpus(); i++)
                workers.push_back(new Worker());
            noWorkers = workers.size();
        }

        virtual ~Dispatcher() {
            noWorkers = 0;
            for (auto* worker: workers)
                delete worker;
            workers.clear();
        };

    public:
        static Dispatcher& getInstance() {
            static Dispatcher MyDispatcher;
            return MyDispatcher;
        }

        FuncId_t registerCB(const Function_t& func, Message_t type) {
            assert(type != Message_t::NONE && type != Message_t::NO_OF_MSG_TYPES);
            std::unique_lock<std::mutex> lock(mutex);
            auto funcId = NextFuncId++;
            cbFuncs.push_back(func);
            cbFuncIds[type].push_back(funcId);
            return funcId;
        }

        void unregisterCB(const FuncId_t& funcId, Message_t type) {
            std::unique_lock<std::mutex> lock(mutex);
            cbFuncs[funcId] = nullptr;
            cbFuncIds[type].remove(funcId);
        }

        void publish(Message* msg) {
            auto msgType = msg->getMsgType();
            if (noWorkers > 0) {
                auto worker = workers[msgType % noWorkers];
                worker->getQueue().push(msg);
            }
        }
    }; // Dispatcher
} // Dispatchers

#endif //CPP_ACTORS_DISPATCHER_H
