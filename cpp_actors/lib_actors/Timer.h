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
#include "Scheduler.h"


namespace Timers
{
    class Timer
    {
    private:
        Schedulers::JobId jobId;
        std::chrono::duration<long, std::milli> msec;
        std::function<void()> func;

    public:
        Timer(std::chrono::duration<long, std::milli> msec, const std::function<void()>& func): jobId(0), msec(msec), func(func) {};
        Timer(long msec, const std::function<void()>& func): Timer(std::chrono::duration<long, std::milli>(msec), func) {};
        virtual ~Timer() = default;

        void stop() {
            if (jobId != 0) {
                Schedulers::Scheduler::getInstance().removeJob(jobId);
                jobId = 0;
            }
        }

        void start() {
            stop();
            jobId = Schedulers::Scheduler::getInstance().onceIn(msec, func);
        }
    }; // Timer
} // Timers

#endif //CPP_ACTORS_TIMER_H
