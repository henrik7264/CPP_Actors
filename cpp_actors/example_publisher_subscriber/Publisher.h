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

#ifndef CPP_ACTORS_PUBLISHER_H
#define CPP_ACTORS_PUBLISHER_H

#include "Messages.h"
#include "Actor.h"

// Only defined to simplify initialization of actors - see main.cpp
#define PUBLISHER() std::shared_ptr<Actors::Actor>(new Actors::Publisher())


namespace Actors
{
    class Publisher: public Actor
    {
    private:
        int i = 0;

    public:
        Publisher(): Actor("PUBLISHER") {
            Scheduler::repeat(1000, [this]() {
                Message::publish(new PubSubMsg("Hello no. " + std::to_string(i++)));
            });
        }
        ~Publisher() override {printf("PUBLISHER called %d times\n", i);};
    }; // Publisher
} // Actors

#endif //CPP_ACTORS_PUBLISHER_H
