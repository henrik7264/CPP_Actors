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

#ifndef CPP_ACTORS_MEMORY_H
#define CPP_ACTORS_MEMORY_H
#include <cstdlib>
#include <chrono>
#include <memory>
#include <list>


namespace MemoryManagement
{
    class MemoryHandler
    {
    private:
        std::list<void*> markedForDeletion;
        std::mutex mutex;

    public:
        MemoryHandler() = default;
        virtual ~MemoryHandler() {freeMarkedMem();}

        void* allocMem(std::size_t sz) noexcept {
            return std::malloc(sz);
        }

        void freeMem(void* ptr) noexcept {
            std::unique_lock<std::mutex> lock(mutex);
            markedForDeletion.push_back(ptr);
        }

        void freeMarkedMem() noexcept {
            std::unique_lock<std::mutex> lock(mutex);
            for (auto* ptr: markedForDeletion)
                std::free(ptr);
            markedForDeletion.clear();
        }
    }; // MemoryHandler

    struct Memory
    {
        static MemoryHandler MyMemory;

        void* operator new(std::size_t sz) noexcept {return MyMemory.allocMem(sz);}
        void* operator new[](std::size_t sz) noexcept {return MyMemory.allocMem(sz);}
        void operator delete(void* ptr) noexcept {MyMemory.freeMem(ptr);}
        void operator delete[](void* ptr) noexcept {MyMemory.freeMem(ptr);}
        static void freeMarkedMem() noexcept {MyMemory.freeMarkedMem();}
    }; // Memory;

    MemoryHandler Memory::MyMemory;
} // MemoryManagement


#endif //CPP_ACTORS_MEMORY_H
