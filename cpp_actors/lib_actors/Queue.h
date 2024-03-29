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

#ifndef CPP_ACTORS_QUEUE_H
#define CPP_ACTORS_QUEUE_H
#include <queue>
#include <mutex>
#include <condition_variable>


namespace Queues
{
    template<typename T>
    class Queue
    {
    private:
        T emptyElem;
        std::queue<T> queue;
        std::mutex mutex;
        std::condition_variable itemAvailable;

    public:
        explicit Queue(const T emptyElem): emptyElem(emptyElem) {};
        Queue(const Queue<T> &) = delete;
        Queue& operator=(const Queue<T>&) = delete;
        virtual ~Queue() = default;

        T get() {
            std::unique_lock<std::mutex> lock(mutex);
            while (queue.empty())
                itemAvailable.wait(lock);
            auto elem = queue.front();
            queue.pop();
            return elem;
        }

        T get(std::chrono::duration<long, std::milli> msec) {
            std::unique_lock<std::mutex> lock(mutex);
            std::cv_status status = std::cv_status::no_timeout;
            while (queue.empty() && (status == std::cv_status::no_timeout))
                status = itemAvailable.wait_for(lock, msec);
            auto elem = emptyElem;
            if (status == std::cv_status::no_timeout) {
                elem = queue.front();
                queue.pop();
            }
            return elem;
        }

        T get(long msec) {
            return get(std::chrono::duration<long, std::milli>(msec));
        }

        void push(const T& item) {
            std::unique_lock<std::mutex> lock(mutex);
            queue.push(item);
            itemAvailable.notify_all();
        }

         size_t size() {
            std::unique_lock<std::mutex> lock(mutex);
            return queue.size();
        }

        bool empty() {
            std::unique_lock<std::mutex> lock(mutex);
            return queue.empty();
        }
    }; // Queue
} // Queues

#endif //CPP_ACTORS_QUEUE_H
