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

#ifndef CPP_ACTORS_TIMER_H
#define CPP_ACTORS_TIMER_H

#include <chrono>
#include <mutex>
#include <functional>
#include <utility>
#include "Scheduler.h"


namespace Timers
{
    class Timer: public MemoryManagement::Memory
    {
    private:
        bool markedForDeletion;
        std::mutex& actorMutex;
        Schedulers::JobId_t jobId;
        std::chrono::duration<long, std::milli> msec;
        Schedulers::Function_t func;

        void timeout() {
            if (!markedForDeletion) {
                std::unique_lock<std::mutex> lock(actorMutex);
                func();
            }
        }

    public:
        Timer(std::mutex& actorMutex, std::chrono::duration<long, std::milli> msec, Schedulers::Function_t func): markedForDeletion(false), actorMutex(actorMutex), jobId(Schedulers::JobIdMax), msec(msec), func(std::move(func)) {}
        Timer(std::mutex& actorMutex, long msec, const Schedulers::Function_t& func): Timer(actorMutex, std::chrono::duration<long, std::milli>(msec), func) {}
        virtual ~Timer() {markedForDeletion = true; stop();}

        void stop() {
            if (jobId != Schedulers::JobIdMax) {
                Schedulers::Scheduler::getInstance().removeJob(jobId);
                jobId = Schedulers::JobIdMax;
            }
        }

        void start() {
            if (!markedForDeletion) {
                stop();
                jobId = Schedulers::Scheduler::getInstance().onceIn(msec, [this](){timeout();});
            }
        }
    }; // Timer
} // Timers

#endif //CPP_ACTORS_TIMER_H
