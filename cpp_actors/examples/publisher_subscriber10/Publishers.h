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

#ifndef CPP_ACTORS_PUBLISHERS_H
#define CPP_ACTORS_PUBLISHERS_H

#include "Messages.h"
#include "Actor.h"

// Only defined to simplify initialization of actors - see main.cpp
#define P0() std::shared_ptr<Actors::Actor>(new Actors::Publisher0())
#define P1() std::shared_ptr<Actors::Actor>(new Actors::Publisher1())
#define P2() std::shared_ptr<Actors::Actor>(new Actors::Publisher2())
#define P3() std::shared_ptr<Actors::Actor>(new Actors::Publisher3())
#define P4() std::shared_ptr<Actors::Actor>(new Actors::Publisher4())
#define P5() std::shared_ptr<Actors::Actor>(new Actors::Publisher5())
#define P6() std::shared_ptr<Actors::Actor>(new Actors::Publisher6())
#define P7() std::shared_ptr<Actors::Actor>(new Actors::Publisher7())
#define P8() std::shared_ptr<Actors::Actor>(new Actors::Publisher8())
#define P9() std::shared_ptr<Actors::Actor>(new Actors::Publisher9())


namespace Actors
{
    class Publisher0: public Actor
    {
    private:
        int i = 0;

    public:
        Publisher0(): Actor("PUBLISHER0") {
            Scheduler::repeat(1, [this]() {
                Messenger::publish(new PubSubMsg0("Hello0 no. " + std::to_string(i++)));
            });
        }
        ~Publisher0() override {printf("PUBLISHER0 called %d times\n", i);};
    }; // Publisher0


    class Publisher1: public Actor
    {
    private:
        int i = 0;

    public:
        Publisher1(): Actor("PUBLISHER1") {
            Scheduler::repeat(1, [this]() {
                Messenger::publish(new PubSubMsg1("Hello1 no. " + std::to_string(i++)));
            });
        }
        ~Publisher1() override {printf("PUBLISHER1 called %d times\n", i);};
    }; // Publisher1


    class Publisher2: public Actor
    {
    private:
        int i = 0;

    public:
        Publisher2(): Actor("PUBLISHER2") {
            Scheduler::repeat(1, [this]() {
                Messenger::publish(new PubSubMsg2("Hello2 no. " + std::to_string(i++)));
            });
        }
        ~Publisher2() override {printf("PUBLISHER2 called %d times\n", i);};
    }; // Publisher2


    class Publisher3: public Actor
    {
    private:
        int i = 0;

    public:
        Publisher3(): Actor("PUBLISHER3") {
            Scheduler::repeat(1, [this]() {
                Messenger::publish(new PubSubMsg3("Hello3 no. " + std::to_string(i++)));
            });
        }
        ~Publisher3() override {printf("PUBLISHER3 called %d times\n", i);};
    }; // Publisher3


    class Publisher4: public Actor
    {
    private:
        int i = 0;

    public:
        Publisher4(): Actor("PUBLISHER4") {
            Scheduler::repeat(1, [this]() {
                Messenger::publish(new PubSubMsg4("Hello4 no. " + std::to_string(i++)));
            });
        }
        ~Publisher4() override {printf("PUBLISHER4 called %d times\n", i);};
    }; // Publisher4


    class Publisher5: public Actor
    {
    private:
        int i = 0;

    public:
        Publisher5(): Actor("PUBLISHER5") {
            Scheduler::repeat(1, [this]() {
                Messenger::publish(new PubSubMsg5("Hello5 no. " + std::to_string(i++)));
            });
        }
        ~Publisher5() override {printf("PUBLISHER5 called %d times\n", i);};
    }; // Publisher5


    class Publisher6: public Actor
    {
    private:
        int i = 0;

    public:
        Publisher6(): Actor("PUBLISHER0") {
            Scheduler::repeat(1, [this]() {
                Messenger::publish(new PubSubMsg6("Hello6 no. " + std::to_string(i++)));
            });
        }
        ~Publisher6() override {printf("PUBLISHER6 called %d times\n", i);};
    }; // Publisher6


    class Publisher7: public Actor
    {
    private:
        int i = 0;

    public:
        Publisher7(): Actor("PUBLISHER7") {
            Scheduler::repeat(1, [this]() {
                Messenger::publish(new PubSubMsg7("Hello7 no. " + std::to_string(i++)));
            });
        }
        ~Publisher7() override {printf("PUBLISHER7 called %d times\n", i);};
    }; // Publisher0


    class Publisher8: public Actor
    {
    private:
        int i = 0;

    public:
        Publisher8(): Actor("PUBLISHER8") {
            Scheduler::repeat(1, [this]() {
                Messenger::publish(new PubSubMsg8("Hello8 no. " + std::to_string(i++)));
            });
        }
        ~Publisher8() override {printf("PUBLISHER8 called %d times\n", i);};
    }; // Publisher0


    class Publisher9: public Actor
    {
    private:
        int i = 0;

    public:
        Publisher9(): Actor("PUBLISHER9") {
            Scheduler::repeat(1, [this]() {
                Messenger::publish(new PubSubMsg9("Hello9 no. " + std::to_string(i++)));
            });
        }
        ~Publisher9() override {printf("PUBLISHER9 called %d times\n", i);};
    }; // Publisher9



} // Actors

#endif //CPP_ACTORS_PUBLISHERS_H
