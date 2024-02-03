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
    class PubSubMsg: public Message
    {
    private:
        std::string data;

    public:
        explicit PubSubMsg(std::string  data): Message(Message_t::PUB_SUB), data(std::move(data)) {}
        ~PubSubMsg() override = default;

        const std::string& getData() const {return data;}
        friend std::ostream& operator<< (std::ostream& out, const PubSubMsg* msg)  {return out << msg->getData();}
    }; // PubSubMsg
} // Messages

#endif //CPP_ACTORS_MESSAGES_H