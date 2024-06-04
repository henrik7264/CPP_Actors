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

#ifndef CPP_ACTORS_SUBSCRIBERS_H
#define CPP_ACTORS_SUBSCRIBERS_H
#include <chrono>
#include <thread>
#include "Messages.h"
#include "Actor.h"


// Only defined to simplify initialization of actors - see main.cpp
#define S0() std::shared_ptr<Actors::Actor>(new Actors::Subscriber0())
#define S1() std::shared_ptr<Actors::Actor>(new Actors::Subscriber1())
#define S2() std::shared_ptr<Actors::Actor>(new Actors::Subscriber2())
#define S3() std::shared_ptr<Actors::Actor>(new Actors::Subscriber3())
#define S4() std::shared_ptr<Actors::Actor>(new Actors::Subscriber4())
#define S5() std::shared_ptr<Actors::Actor>(new Actors::Subscriber5())
#define S6() std::shared_ptr<Actors::Actor>(new Actors::Subscriber6())
#define S7() std::shared_ptr<Actors::Actor>(new Actors::Subscriber7())
#define S8() std::shared_ptr<Actors::Actor>(new Actors::Subscriber8())
#define S9() std::shared_ptr<Actors::Actor>(new Actors::Subscriber9())


namespace Actors
{
    class Subscriber0: public Actor
    {
    private:
        int i = 0;

    public:
        Subscriber0(): Actor("SUBSCRIBER0")
        {
            Messenger::subscribe(Message_t::PUB_SUB0, [this](Message* msg) {
                auto* pubSubMsg = dynamic_cast<PubSubMsg0*>(msg);
                Logger::debug() << pubSubMsg;
                i++;
            });
        }
        ~Subscriber0() override {printf("SUBSCRIBER0 called %d times\n", i);}
    }; // Subscriber0


    class Subscriber1: public Actor
    {
    private:
        int i = 0;

    public:
        Subscriber1(): Actor("SUBSCRIBER1")
        {
            Messenger::subscribe(Message_t::PUB_SUB1, [this](Message* msg) {
                auto* pubSubMsg = dynamic_cast<PubSubMsg1*>(msg);
                Logger::debug() << pubSubMsg;
                i++;
            });
        }
        ~Subscriber1() override {printf("SUBSCRIBER1 called %d times\n", i);}
    }; // Subscriber1


    class Subscriber2: public Actor
    {
    private:
        int i = 0;

    public:
        Subscriber2(): Actor("SUBSCRIBER2")
        {
            Messenger::subscribe(Message_t::PUB_SUB2, [this](Message* msg) {
                auto* pubSubMsg = dynamic_cast<PubSubMsg2*>(msg);
                Logger::debug() << pubSubMsg;
                i++;
            });
        }
        ~Subscriber2() override {printf("SUBSCRIBER2 called %d times\n", i);}
    }; // Subscriber2


    class Subscriber3: public Actor
    {
    private:
        int i = 0;

    public:
        Subscriber3(): Actor("SUBSCRIBER3")
        {
            Messenger::subscribe(Message_t::PUB_SUB3, [this](Message* msg) {
                auto* pubSubMsg = dynamic_cast<PubSubMsg3*>(msg);
                Logger::debug() << pubSubMsg;
                i++;
            });
        }
        ~Subscriber3() override {printf("SUBSCRIBER3 called %d times\n", i);}
    }; // Subscriber3


    class Subscriber4: public Actor
    {
    private:
        int i = 0;

    public:
        Subscriber4(): Actor("SUBSCRIBER4")
        {
            Messenger::subscribe(Message_t::PUB_SUB4, [this](Message* msg) {
                auto* pubSubMsg = dynamic_cast<PubSubMsg4*>(msg);
                Logger::debug() << pubSubMsg;
                i++;
            });
        }
        ~Subscriber4() override {printf("SUBSCRIBER4 called %d times\n", i);}
    }; // Subscriber4


    class Subscriber5: public Actor
    {
    private:
        int i = 0;

    public:
        Subscriber5(): Actor("SUBSCRIBER5")
        {
            Messenger::subscribe(Message_t::PUB_SUB5, [this](Message* msg) {
                auto* pubSubMsg = dynamic_cast<PubSubMsg5*>(msg);
                Logger::debug() << pubSubMsg;
                i++;
            });
        }
        ~Subscriber5() override {printf("SUBSCRIBER5 called %d times\n", i);}
    }; // Subscriber5


    class Subscriber6: public Actor
    {
    private:
        int i = 0;

    public:
        Subscriber6() : Actor("SUBSCRIBER6")
        {
            Messenger::subscribe(Message_t::PUB_SUB6, [this](Message* msg) {
                auto* pubSubMsg = dynamic_cast<PubSubMsg6*>(msg);
                Logger::debug() << pubSubMsg;
                i++;
            });
        }
        ~Subscriber6() override {printf("SUBSCRIBER6 called %d times\n", i);}
    }; // Subscriber6


    class Subscriber7: public Actor
    {
    private:
        int i = 0;

    public:
        Subscriber7(): Actor("SUBSCRIBER7")
        {
            Messenger::subscribe(Message_t::PUB_SUB7, [this](Message* msg) {
                auto* pubSubMsg = dynamic_cast<PubSubMsg7*>(msg);
                Logger::debug() << pubSubMsg;
                i++;
            });
        }
        ~Subscriber7() override {printf("SUBSCRIBER7 called %d times\n", i);}
    }; // Subscriber7


    class Subscriber8: public Actor
    {
    private:
        int i = 0;

    public:
        Subscriber8(): Actor("SUBSCRIBER8")
        {
            Messenger::subscribe(Message_t::PUB_SUB8, [this](Message* msg) {
                auto* pubSubMsg = dynamic_cast<PubSubMsg8*>(msg);
                Logger::debug() << pubSubMsg;
                i++;
            });
        }
        ~Subscriber8() override {printf("SUBSCRIBER8 called %d times\n", i);}
    }; // Subscriber8


    class Subscriber9: public Actor
    {
    private:
        int i = 0;

    public:
        Subscriber9() : Actor("SUBSCRIBER9")
        {
            Messenger::subscribe(Message_t::PUB_SUB9, [this](Message* msg) {
                auto* pubSubMsg = dynamic_cast<PubSubMsg9*>(msg);
                Logger::debug() << pubSubMsg;
                i++;
            });
        }
        ~Subscriber9() override {printf("SUBSCRIBER9 called %d times\n", i);}
    }; // Subscriber9


} // Actors

#endif //CPP_ACTORS_SUBSCRIBERS_H
