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

#ifndef CPP_ACTORS_SUBSCRIBER6_H
#define CPP_ACTORS_SUBSCRIBER6_H
#include <chrono>
#include <thread>
#include "Messages.h"
#include "Actor.h"

// Only defined to simplify initialization of actors - see main.cpp
#define SUBSCRIBER6() std::shared_ptr<Actors::Actor>(new Actors::Subscriber6())


namespace Actors
{
    class Subscriber6: public Actor
    {
    private:
        int i = 0;

    public:
        Subscriber6() : Actor("SUBSCRIBER6")
        {
            Messenger::subscribe(Message_t::PUB_SUB, [this](Message* msg) {
                auto* pubSubMsg = dynamic_cast<PubSubMsg*>(msg);
                Logger::debug() << pubSubMsg;
                i++;
            });
        }
        ~Subscriber6() override {printf("SUBSCRIBER6 called %d times\n", i);}
    }; // Subscriber
} // Actors

#endif //CPP_ACTORS_SUBSCRIBER6_H
