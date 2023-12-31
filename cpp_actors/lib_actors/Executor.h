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
#include <climits>
#include <functional>
#include <memory>
#include <list>
#include <thread>
#include <mutex>
#include "Queue.h"
#include "Message.h"

using namespace Messages;


namespace Executors
{
    static Queues::Queue<std::pair<std::function<void(Message_ptr)>, Message_ptr>> jobQueue;
    static Message_ptr noneMsg = std::make_shared<Message>(MessageType::NONE);

    class Worker
    {
    private:
        bool doLoop = true;
        bool done = false;
        std::thread trd;

        void run() {
            while (doLoop) {
                auto job = jobQueue.get();
                job.first(job.second);
            }
            done = true;
        }

        void stop() {
            doLoop = false;
            auto timeout = 1; //ms
            while (!done) {
                jobQueue.push(std::make_pair([](const Message_ptr &) {}, noneMsg));
                std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
                if (timeout <= 512)
                    timeout *= 2;
            }
            if (trd.joinable())
                trd.join();
        }

    public:
        Worker() : trd(std::thread([this]() { run(); })) {}
        virtual ~Worker() {stop();}

        void stopWorker() {doLoop = false;}
    }; // Worker

    class Executor
    {
    private:
        std:: mutex mutex;
        std::list<Worker*> workers = {new Worker()};
        int maxQueueSize = 2;
        bool doExec = true;

        Executor() = default;
        virtual ~Executor() {
            std::unique_lock<std::mutex> lock(mutex);
            printf("Executor::~Executor: workers: %zu, qsize: %zu\n", workers.size(), jobQueue.size());
            doExec = false;
            for (auto worker: workers)
                delete (worker);
            workers.clear();
        };

    public:
        static Executor& getInstance() {
            static Executor MyExecutor;
            return MyExecutor;
        }

        void exec(const std::function<void(Message_ptr)>& func, const Message_ptr& arg) {
            std::unique_lock<std::mutex> lock(mutex);
            if (doExec) {
                jobQueue.push(std::make_pair(func, arg));
                if (jobQueue.size() > maxQueueSize) {
                    workers.push_back(new Worker());
                    maxQueueSize *= 2;
                }
            }
        }

        void exec(const std::function<void()>& func) {
            std::unique_lock<std::mutex> lock(mutex);
            if (doExec) {
                jobQueue.push(std::make_pair([func](const Message_ptr &) { func(); }, noneMsg));
                if (jobQueue.size() > maxQueueSize) {
                    workers.push_back(new Worker());
                    maxQueueSize *= 2;
                }
            }
        }

        void stopExecutor() {
            std::unique_lock<std::mutex> lock(mutex);
            doExec = false;
            for (auto worker: workers)
                worker->stopWorker();
        }
    }; // Executor
} // Executors

#endif //CPP_ACTORS_EXECUTOR_H
