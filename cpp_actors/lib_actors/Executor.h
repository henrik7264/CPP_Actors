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

#ifndef CPP_ACTORS_EXECUTOR_H
#define CPP_ACTORS_EXECUTOR_H
#include <functional>
#include <memory>
#include <list>
#include <thread>
#include "Queue.h"
#include "Message.h"

using namespace Messages;


namespace Executors
{
    static Queues::Queue<std::pair<std::function<void(Message_ptr)>, Message_ptr>> jobQueue;

    class Worker
    {
    private:
        bool doLoop = true;
        std::thread trd;

        void run() const {
            while (doLoop) {
                auto job = jobQueue.get();
                job.first(job.second);
            }
        }

        void stop() {
            assert(!doLoop);
            if (trd.joinable())
                trd.join();
        }

    public:
        Worker() : trd(std::thread([this]() { run(); })) {}
        virtual ~Worker() { stop(); }

        void stopLoop() { doLoop = false; }

        static void exitLoop() {
            jobQueue.push(std::make_pair([](const Message_ptr &) {}, std::make_shared<Message>(MessageType::NONE)));
        }
    }; // Worker

    class Executor
    {
    private:
        std::list<Worker*> workers = {new Worker()};
        int maxQueueSize = 2;

        Executor() = default;
        virtual ~Executor() {
            printf("Executor workers in use: %zu\n", workers.size());
            for (auto worker: workers)
                worker->stopLoop();
            for (auto worker: workers)
                worker->exitLoop();
            for (auto worker: workers)
                delete (worker);
        };

    public:
        static Executor& getInstance() {
            static Executor MyExecutor;
            return MyExecutor;
        }

        void exec(const std::function<void(Message_ptr)>& func, const Message_ptr& arg) {
            jobQueue.push(std::make_pair(func, arg));
            if (jobQueue.size() > maxQueueSize) {
                workers.push_back(new Worker());
                maxQueueSize *= 2;
            }
        }

        void exec(const std::function<void()>& func) {
            jobQueue.push(std::make_pair([func](const Message_ptr &){func();}, std::make_shared<Message>(MessageType::NONE)));
            if (jobQueue.size() > maxQueueSize) {
                workers.push_back(new Worker());
                maxQueueSize *= 2;
            }
        }
    }; // Executor
} // Executors

#endif //CPP_ACTORS_EXECUTOR_H
