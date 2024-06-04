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

#ifndef CPP_ACTORS_MESSAGES_H
#define CPP_ACTORS_MESSAGES_H

#include <iostream>
#include <string>
#include "Message.h"


namespace Messages
{
    class PubSubMsg0: public Message
    {
    private:
        std::string data;

    public:
        explicit PubSubMsg0(std::string  data): Message(Message_t::PUB_SUB0), data(std::move(data)) {}
        ~PubSubMsg0() override = default;

        const std::string& getData() const {return data;}
        friend std::ostream& operator<< (std::ostream& out, const PubSubMsg0* msg)  {return out << msg->getData();}
    }; // PubSubMsg0


    class PubSubMsg1: public Message
    {
    private:
        std::string data;

    public:
        explicit PubSubMsg1(std::string  data): Message(Message_t::PUB_SUB1), data(std::move(data)) {}
        ~PubSubMsg1() override = default;

        const std::string& getData() const {return data;}
        friend std::ostream& operator<< (std::ostream& out, const PubSubMsg1* msg)  {return out << msg->getData();}
    }; // PubSubMsg1


    class PubSubMsg2: public Message
    {
    private:
        std::string data;

    public:
        explicit PubSubMsg2(std::string  data): Message(Message_t::PUB_SUB2), data(std::move(data)) {}
        ~PubSubMsg2() override = default;

        const std::string& getData() const {return data;}
        friend std::ostream& operator<< (std::ostream& out, const PubSubMsg2* msg)  {return out << msg->getData();}
    }; // PubSubMsg2


    class PubSubMsg3: public Message
    {
    private:
        std::string data;

    public:
        explicit PubSubMsg3(std::string  data): Message(Message_t::PUB_SUB3), data(std::move(data)) {}
        ~PubSubMsg3() override = default;

        const std::string& getData() const {return data;}
        friend std::ostream& operator<< (std::ostream& out, const PubSubMsg3* msg)  {return out << msg->getData();}
    }; // PubSubMsg3


    class PubSubMsg4: public Message
    {
    private:
        std::string data;

    public:
        explicit PubSubMsg4(std::string  data): Message(Message_t::PUB_SUB4), data(std::move(data)) {}
        ~PubSubMsg4() override = default;

        const std::string& getData() const {return data;}
        friend std::ostream& operator<< (std::ostream& out, const PubSubMsg4* msg)  {return out << msg->getData();}
    }; // PubSubMsg4


    class PubSubMsg5: public Message
    {
    private:
        std::string data;

    public:
        explicit PubSubMsg5(std::string  data): Message(Message_t::PUB_SUB5), data(std::move(data)) {}
        ~PubSubMsg5() override = default;

        const std::string& getData() const {return data;}
        friend std::ostream& operator<< (std::ostream& out, const PubSubMsg5* msg)  {return out << msg->getData();}
    }; // PubSubMsg5


    class PubSubMsg6: public Message
    {
    private:
        std::string data;

    public:
        explicit PubSubMsg6(std::string  data): Message(Message_t::PUB_SUB6), data(std::move(data)) {}
        ~PubSubMsg6() override = default;

        const std::string& getData() const {return data;}
        friend std::ostream& operator<< (std::ostream& out, const PubSubMsg6* msg)  {return out << msg->getData();}
    }; // PubSubMsg6


    class PubSubMsg7: public Message
    {
    private:
        std::string data;

    public:
        explicit PubSubMsg7(std::string  data): Message(Message_t::PUB_SUB7), data(std::move(data)) {}
        ~PubSubMsg7() override = default;

        const std::string& getData() const {return data;}
        friend std::ostream& operator<< (std::ostream& out, const PubSubMsg7* msg)  {return out << msg->getData();}
    }; // PubSubMsg7


    class PubSubMsg8: public Message
    {
    private:
        std::string data;

    public:
        explicit PubSubMsg8(std::string  data): Message(Message_t::PUB_SUB8), data(std::move(data)) {}
        ~PubSubMsg8() override = default;

        const std::string& getData() const {return data;}
        friend std::ostream& operator<< (std::ostream& out, const PubSubMsg8* msg)  {return out << msg->getData();}
    }; // PubSubMsg8

 
    class PubSubMsg9: public Message
    {
    private:
        std::string data;

    public:
        explicit PubSubMsg9(std::string  data): Message(Message_t::PUB_SUB9), data(std::move(data)) {}
        ~PubSubMsg9() override = default;

        const std::string& getData() const {return data;}
        friend std::ostream& operator<< (std::ostream& out, const PubSubMsg9* msg)  {return out << msg->getData();}
    }; // PubSubMsg9
} // Messages



#endif //CPP_ACTORS_MESSAGES_H