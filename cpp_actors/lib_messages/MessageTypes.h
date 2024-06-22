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


#ifndef CPP_ACTORS_MESSAGETYPES_H
#define CPP_ACTORS_MESSAGETYPES_H


namespace Messages
{
    enum Message_t
    {
        NONE, // Don't remove or rename. NONE shall always be the first element.
        PUB_SUB, // Used in example publish subscriber.
<<<<<<< Updated upstream
        PUB_SUB0, // Used in example publish subscriber10.
        PUB_SUB1, // Used in example publish subscriber10.
        PUB_SUB2, // Used in example publish subscriber10.
        PUB_SUB3, // Used in example publish subscriber10.
        PUB_SUB4, // Used in example publish subscriber10.
        PUB_SUB5, // Used in example publish subscriber10.
        PUB_SUB6, // Used in example publish subscriber10.
        PUB_SUB7, // Used in example publish subscriber10.
        PUB_SUB8, // Used in example publish subscriber10.
        PUB_SUB9, // Used in example publish subscriber10.
=======
        PUB_SUB0, // Used in example publish subscribers.
        PUB_SUB1, // Used in example publish subscribers.
        PUB_SUB2, // Used in example publish subscribers.
        PUB_SUB3, // Used in example publish subscribers.
        PUB_SUB4, // Used in example publish subscribers.
        PUB_SUB5, // Used in example publish subscribers.
        PUB_SUB6, // Used in example publish subscribers.
        PUB_SUB7, // Used in example publish subscribers.
        PUB_SUB8, // Used in example publish subscribers.
        PUB_SUB9, // Used in example publish subscribers.
>>>>>>> Stashed changes
        OPEN_DOOR, // Used in example state machine.
        CLOSE_DOOR, // Used in example state machine.
        NO_OF_MSG_TYPES // Don't remove or rename. NO_OF_MSG_TYPES shall always be the last element.
    };
} // Messages

#endif //CPP_ACTORS_MESSAGETYPES_H
