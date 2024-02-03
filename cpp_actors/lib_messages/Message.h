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

#ifndef CPP_ACTORS_MESSAGE_H
#define CPP_ACTORS_MESSAGE_H
#include <memory>
#include "MessageTypes.h"


namespace Messages
{
    class Message
    {
    private:
        Message_t msgType;

    public:
        explicit Message(Message_t type): msgType(type) {}
        virtual ~Message() = default;

        Message_t getMsgType() const {return msgType;}
    }; // Message

    typedef std::shared_ptr<Message> Message_ptr;
} // Messages

#endif //CPP_ACTORS_MESSAGE_H
