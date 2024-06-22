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
#include "Memory.h"
#include "Scheduler.h"


namespace Timers
{
    class Timer
    {
    private:
        std::mutex& actorMutex;
        std::mutex timerMutex;
        Schedulers::JobId_t jobId;
        std::chrono::duration<long, std::milli> msec;
        Schedulers::Function_t func;

        void timeout() {
            std::unique_lock<std::mutex> lock(actorMutex);
<<<<<<< Updated upstream
            func();
=======
            if (!markedForDeletion)
                func();
>>>>>>> Stashed changes
        }

        void stopTimer() {
            if (jobId != Schedulers::JobIdMax) {
                Schedulers::Scheduler::getInstance().removeJob(jobId);
                jobId = Schedulers::JobIdMax;
            }
        }

    public:
        Timer(std::mutex& actorMutex, std::chrono::duration<long, std::milli> msec, Schedulers::Function_t func): actorMutex(actorMutex), jobId(Schedulers::JobIdMax), msec(msec), func(std::move(func)) {}
        Timer(std::mutex& actorMutex, long msec, const Schedulers::Function_t& func): Timer(actorMutex, std::chrono::duration<long, std::milli>(msec), func) {}
        virtual ~Timer() {
            std::unique_lock<std::mutex> lock(actorMutex);
<<<<<<< Updated upstream
=======
            markedForDeletion = true;
>>>>>>> Stashed changes
            stopTimer();
        }

        void stop() {
            std::unique_lock<std::mutex> lock(timerMutex);
            stopTimer();
        }

        void start() {
            std::unique_lock<std::mutex> lock(timerMutex);
<<<<<<< Updated upstream
            stopTimer();
            jobId = Schedulers::Scheduler::getInstance().onceIn(msec, [this](){timeout();});
=======
            if (!markedForDeletion) {
                stopTimer();
                jobId = Schedulers::Scheduler::getInstance().onceIn(msec, [this](){timeout();});
            }
>>>>>>> Stashed changes
        }
    }; // Timer
} // Timers

#endif //CPP_ACTORS_TIMER_H
