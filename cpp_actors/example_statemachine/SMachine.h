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

#ifndef CPP_ACTORS_SMACHINE_H
#define CPP_ACTORS_SMACHINE_H
#include "Messages.h"
#include "Actor.h"

// Only defined to simplify initialization of actors - see main.cpp
#define SMACHINE() std::shared_ptr<Actors::Actor>(new Actors::SMachine())

namespace Actors
{
    class SMachine: public Actor
    {
    private:
        enum State {DOOR_OPENED, DOOR_CLOSED};
        StateMachine_t sm = STATEMACHINE(State::DOOR_CLOSED,
                STATE(State::DOOR_CLOSED,
                      MESSAGE(Message_t::OPEN_DOOR, NEXT_STATE(State::DOOR_OPENED), [this](Message* msg){
                          Logger::debug() << "Opening door ...";})),
                STATE(State::DOOR_OPENED,
                      MESSAGE(Message_t::CLOSE_DOOR, NEXT_STATE(State::DOOR_CLOSED), [this](Message* msg){
                          Logger::debug() << "Closing door ...";}),
                      TIMER(1000, NEXT_STATE(State::DOOR_CLOSED), [this](){
                          Logger::debug() << "Auto closing door ...";})));

    public:
        SMachine(): Actor("SMACHINE") {}
        ~SMachine() override = default;
    }; // SMachine
} // Actors

#endif //CPP_ACTORS_SMACHINE_H
