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


#ifndef CPP_ACTORS_SCHEDULER_H
#define CPP_ACTORS_SCHEDULER_H
#include <cassert>
#include <climits>
#include <functional>
#include <tuple>
#include <list>
#include <map>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Queue.h"
#include "Executor.h"


namespace Schedulers
{
    typedef unsigned long JobId;

    class Scheduler
    {
    private:
        bool doLoop = true;
        JobId jobId = 0;
        std::map<JobId, std::tuple<std::chrono::steady_clock::time_point, std::chrono::duration<long,std::milli>, std::function<void()>, unsigned int>> jobs;
        std::mutex mutex;
        std::condition_variable condVar;
        std::thread trd;

        Scheduler(): trd(std::thread([this]() {run();})) {}
        virtual ~Scheduler() {stop();}

        void run() {
            while (doLoop) {
                std::unique_lock<std::mutex> lock(mutex);
                if (jobs.empty())
                    condVar.wait(lock);
                else {
                    auto nextTimeout = std::chrono::time_point<std::chrono::steady_clock>::max();
                    for (auto job: jobs) {
                        auto jobTimeout = std::get<0>(job.second);
                        auto jobRepeat = std::get<3>(job.second);
                        if (jobRepeat > 0 && jobTimeout < nextTimeout)
                            nextTimeout = jobTimeout;
                    }
                    if (nextTimeout > std::chrono::steady_clock::now())
                        condVar.wait_until(lock, nextTimeout);
                }

                auto currentTime = std::chrono::steady_clock::now();
                for (auto job: jobs) {
                    auto id = job.first;
                    auto jobTimeout = std::get<0>(job.second);
                    auto jobDuration = std::get<1>(job.second);
                    auto jobFunction = std::get<2>(job.second);
                    auto jobRepeat = std::get<3>(job.second);

                    if (jobRepeat > 0 && jobTimeout <= currentTime) {
                        Executors::Executor::getInstance().exec(jobFunction);
                        jobs[id] = std::make_tuple(jobTimeout + jobDuration, jobDuration, jobFunction, jobRepeat - 1);
                    }
                }
            }
        }

        void stop() {
            doLoop = false;
            condVar.notify_one();
            if (trd.joinable())
                trd.join();
        }

    public:
        static Scheduler& getInstance() {
            static Scheduler MyScheduler;
            return MyScheduler;
        }

        JobId onceIn(std::chrono::duration<long, std::milli> msec, const std::function<void()>& func) {
            std::unique_lock<std::mutex> lock(mutex);
            jobId++;
            jobs[jobId] = std::make_tuple(std::chrono::steady_clock::now()+msec, msec, func, 1);
            condVar.notify_one();
            return jobId;
        }

        JobId onceIn(long msec, const std::function<void()>& func) {
            return onceIn(std::chrono::duration<long, std::milli>(msec), func);
        }

        JobId repeatEvery(std::chrono::duration<long, std::milli> msec, const std::function<void()>& func) {
            std::unique_lock<std::mutex> lock(mutex);
            jobId++;
            jobs[jobId] = std::make_tuple(std::chrono::steady_clock::now()+msec, msec, func, UINT_MAX);
            condVar.notify_one();
            return jobId;
        }

        JobId repeatEvery(long msec, const std::function<void()>& func) {
            return repeatEvery(std::chrono::duration<long, std::milli>(msec), func);
        }

        void removeJob(JobId id) {
            std::unique_lock<std::mutex> lock(mutex);
            if (jobs.find(id) != jobs.end()) {
                jobs.erase(id);
                condVar.notify_one();
            }
        }
    }; // Scheduler
} // Schedulers

#endif //CPP_ACTORS_SCHEDULER_H
