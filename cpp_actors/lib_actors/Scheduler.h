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


namespace Schedulers
{
    typedef std::function<void()> Function_t;
    typedef unsigned long JobId_t;
    static JobId_t NextJobId = 0;
    static const JobId_t JobIdMax = ULONG_MAX;
    typedef unsigned long RepeatTimes_t;
    static const JobId_t RepeatTimesMax = ULONG_MAX;

    class Worker
    {
    private:
        bool doLoop = true;
        std::thread trd;
        Queues::Queue <Function_t> jobQueue{nullptr};

        void run()
        {
            while (doLoop) {
                auto func = jobQueue.get(100);
                if (doLoop && func)
                    func();
            }
        }

        void stop() {
            doLoop = false;
            if (trd.joinable())
                trd.join();
        }

    public:
        Worker() { trd = std::thread([this]() { run(); }); }
        virtual ~Worker() { stop(); }

        inline Queues::Queue <Function_t>& getQueue() { return jobQueue; }
    }; // Worker


    class Scheduler
    {
    private:
        bool doLoop = true;
        std::thread trd;
        std::mutex mutex;
        std::condition_variable jobAvailable;
        std::map<JobId_t, std::tuple<Function_t, std::chrono::steady_clock::time_point, std::chrono::duration<long,std::milli>, RepeatTimes_t>> jobs;
        Worker* worker = new Worker();
        std::size_t noWorkers = 1;

        void run() {
            while (doLoop) {
                std::unique_lock<std::mutex> lock(mutex);
                while (doLoop && jobs.empty())
                    jobAvailable.wait(lock);

                auto nextTimeout = std::chrono::time_point<std::chrono::steady_clock>::max();
                for (auto job: jobs) {
                    auto jobTimeout = std::get<1>(job.second);
                    auto jobRepeatTimes = std::get<3>(job.second);
                    if ((jobRepeatTimes > 0) && (jobTimeout < nextTimeout))
                        nextTimeout = jobTimeout;
                }
                if (doLoop && (nextTimeout > std::chrono::steady_clock::now()))
                    jobAvailable.wait_until(lock, nextTimeout);

                std::list<JobId_t> jobsToRemove;
                auto currentTime = std::chrono::steady_clock::now();
                for (auto job: jobs) {
                    auto jobId = job.first;
                    auto func = std::get<0>(job.second);
                    auto jobTimeout = std::get<1>(job.second);
                    auto jobDuration = std::get<2>(job.second);
                    auto jobRepeatTimes = std::get<3>(job.second);

                    if (doLoop && (noWorkers > 0) && (jobRepeatTimes > 0) && (jobTimeout <= currentTime)) {
                        worker->getQueue().push(func);
                        if (jobRepeatTimes == 1)
                            jobsToRemove.push_back(jobId);
                        else
                            jobs[jobId] = std::make_tuple(func, jobTimeout + jobDuration, jobDuration, jobRepeatTimes-1);
                    }
                }
                for (auto jobId: jobsToRemove)
                    jobs.erase(jobId);
            }
        }

        void stop() {
            doLoop = false;
            jobAvailable.notify_one();
            if (trd.joinable())
                trd.join();
        }

        Scheduler() {trd = std::thread([this]() {run();});}
        virtual ~Scheduler() {
            stop();
            delete worker;
            noWorkers = 0;
        }

    public:
        static Scheduler& getInstance() {
            static Scheduler MyScheduler;
            return MyScheduler;
        }

        JobId_t onceIn(std::chrono::duration<long, std::milli> msec, const Function_t& func) {
            std::unique_lock<std::mutex> lock(mutex);
            auto jobId = NextJobId++;
            jobs[jobId] = std::make_tuple(func, std::chrono::steady_clock::now()+msec, msec, 1);
            jobAvailable.notify_one();
            return jobId;
        }

        JobId_t onceIn(long msec, const Function_t& func) {
            return onceIn(std::chrono::duration<long, std::milli>(msec), func);
        }

        JobId_t repeatEvery(std::chrono::duration<long, std::milli> msec, const std::function<void()>& func) {
            std::unique_lock<std::mutex> lock(mutex);
            auto jobId = NextJobId++;
            jobs[jobId] = std::make_tuple(func, std::chrono::steady_clock::now()+msec, msec, RepeatTimesMax);
            jobAvailable.notify_one();
            return jobId;
        }

        JobId_t repeatEvery(long msec, const Function_t& func) {
            return repeatEvery(std::chrono::duration<long, std::milli>(msec), func);
        }

        void removeJob(const JobId_t jobId) {
            std::unique_lock<std::mutex> lock(mutex);
            if (jobs.find(jobId) != jobs.end()) {
                jobs.erase(jobId);
                jobAvailable.notify_one();
            }
        }
    }; // Scheduler
} // Schedulers
#endif //CPP_ACTORS_SCHEDULER_H
