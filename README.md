# Reactive Systems using Actors

Dear all,

Welcome to Actors - a C++ library for creating Reactive Systems based on the Actors model.

## Status

The library is currently a work in progress and may change without any notice.
It's a proof of concept version that provides the basic Actors functionality 
and most of the features discussed below. The library has been tested informally on a

* Portable PC (i7-9750H CPU, Ubuntu 22.04, g++ v11.3.0)
* High end PC (i9-12900k CPU, Fedora 39, g++ v13.2.1)

## Idea

The idea with this library is to create a framework for creating highly performant, scalable and maintainable code.
My inspiration for this project originates from the following sources:

* Actors Model (https://en.wikipedia.org/wiki/Actor_model)
* ROS (https://www.ros.org)
* RxROS (https://github.com/rosin-project/rxros)
* RxROS2 (https://github.com/rosin-project/rxros2)
* Reactive Programming (https://reactivex.io)
* Reactive systems (https://www.reactivemanifesto.org)
* PY_Actors (https://github.com/henrik7264/PY_Actors)

Although there are several references to ROS, the library has as such nothing to do with programming robots.
It is a general purpose library/framework for creating reactive systems using Actors.

## Approach

I have always found the following items central for producing high quality software

* Low coupling/high cohesion<br>
  Low coupling/high cohesion is a central part of the Actors model and therefore also a central part of this library.
  The only way Actors can communicate with each other is through messages.
  An actor can publish a message and scribe and react to published messages.
  Each and every actor lives it own life. It will newer break due to major changes in other Actors.
  The developer can concentrate on developing the best Actor in the wold without ever having to think of how other Actors are implemented.
  An Actor may start other actors to perform its task. The only dependency between Actors are the messages and the properties/data they carries –
  so designing a system based on Actors is all about creating proper messages that can be distributed and processed by other Actors.
* Reusability<br>Reusability of code is the use of existing software, or software knowledge,
  to build new software (ref. https://en.wikipedia.org/wiki/Code_reuse). This is of course the fundamental idea with library.
  I just hope the library has the quality that is needed to be used over and over again in many projects.
  The big question is however if the produced code using the library will result in more reusable code?
  This is my hope, but it will have to be seen.
* Keep it simple<br>Keep it simple! Well, well, well - I could talk hours about this subject,
  but this is not the purpose of this section. I think you know what I am talking about,
  especially if you have tried to take over the maintenance of some code your "dear" colleague produced just before he left the company.
  Even code that I produced myself is hard to read and understand after having been away from it for say ½ a year.
  I have put a lot of work in making this code easy to read and understand,
  but it is in the usage of the library that the "keep it simple" statement really should shine through.

It is my hope that the above items are reflected in the code and especially the usage it,
and that the library provides a solid foundation for creating reactive systems based on Actors.

## Project phases

I foresee at least two phases for the library. Phase1 is related to only adding features to the library. This includes:

* Logging<br>Logging is one of the most fundamental debugging facilities a library like this must provide.
  It shall be easy to enable and use. A log entry shall contain a time stamp,
  severity, which Actor created the entry and a text that describes a problem or information about the state of the Actor.
* Http<br>A Web page shall be available for each Actor. The purpose of the web page is to provide monitoring of an Actor.
  More advanced web pages that collects information on the overall application (Actors), will be added later.
* Statistics<br>Statistics is like logging essential for debugging a program.
  It shall be possible to see how many messages of a given type have been published
  and how long time has gone sine we received the last message.
  Queue sizes and number of worker threads should also be information that easily can be shown.
  All these information could in principle be shown in a web page. This Will be decided at a later time.
* Scheduling and Timers<br>Scheduling and timers are fundamental of all systems and shall as such be part of this library.
* State machines<br>State machines are part of nearly all systems. However, they come in many variants/implementations
  and it is always complicated to understand how they work and what they do.
  A simple approach to state machines will be presented in this library.
  The implementation will be very close to the definition of a state machine.
* Behavior trees<br>Experimental feature that will be added to the library.
* Reactive programming<br>Experimental feature that will be added to the library.
  Instead of looking at a system that is based on parsing messages from one Actor to another,
  we could also look at it as system of message streams that are processed by the Actors.
  When we take this view reactive programming becomes a natural way of processing the messages.
  See some of the advantages of using reactive programming on ttps://reactivex.io - they are awesome.
  
<p align="center">
  <img src="https://github.com/henrik7264/Actors/blob/main/images/Actors_Phase1.png" height="400"><br>
  Phase1: Focus is to provide different features for Actors.
</p>

The second phase is only related to create a distributed system of Actors that can communicate with each other on multiple platforms and hosts.

<p align="center">
  <img src="https://github.com/henrik7264/Actors/blob/main/images/Actors_Phase2.png" height="400"><br>
  Phase2: Focus is to provide a distributed environment for sharing messages between Actors.
</p>

The progress of this library depends a lot on the interest for it and if the overall goals actually are met.

## Required software

The Actors library depends on the following software:

* C++ compiler (must support c++ standard 17)
* CMake (v3.22, earlier versions are likely to work)
* RxCPP v2 (see https://github.com/ReactiveX/RxCPP)

## Installation and setup

The Actors library depends on the ReactiveX extensions RxCPP
This extensions must be installed prior to installing the library.
The installation process is as follows:

1. Install RxCPP
2. Install Actors library

### Installation of RxCPP on Linux

```bash
git clone --recursive https://github.com/ReactiveX/RxCpp.git
cd RxCpp
mkdir build
cd build
cmake ..
sudo make install 
```

### Installation of the Actors library on Linux

```bash
git clone https://github.com/henrik7264/CPP_Actors.git
```

#### Testing the C++ library on Linux

```bash
cd CPP_Actors/cpp_actors
mkdir build
cd build
cmake ..
make
./example_pubsub
./example_smachine
```

## Using the Actors library in your own project

Now to the more fun part of using the Actors library.

### Project setup

There is currently no installation packages for the Actors library.
The code is simply indented to be included directly in your project.
Copy the lib_actors, lib_messages and CMakeLists.txt folders directly
into your project and start to create new actors as described below. 
The CMakeLists.txt will have to be updated to reflect your project.
It should be relative easy as the file is quite simple.

```bash
YouProject/
  lib_actors/
  lib_messages/
  ...
  actor1/
  actor2/
  ...
  main.cpp
  CMakeLists.txt
```

A number of examples are provided as part of the Actors library.
They should provide enough information to setup your development environment.

## Features of the Actor library

The following sections describe messages, actors, schedulers, timers and state machines. As the library expands new features will be added

### Messages

Messages are one of the most important concepts of the Actors library.
A message is simply a class, or more precisely a subclass of the Message class.
The constructor of the Message class takes a MessageType as argument.
The Messages type are defined in the MessageTpe.h file and must be updated
with every used message type.

The most simple message consists of nothing but a class/struct definition:

```cpp
#include "Message.h"

namespace Messages
{
    struct SimpleMsg: public Message
    {
        SimpleMsg(): Message(MessageType::SIMPLE_MSG) {}
        ~SimpleMsg() override = default;
    }; // SimpleMsg
} // Messages
```

Remember to update the MessageType.h file as follows:

```cpp
namespace Messages
{
    enum MessageType
    {
        NONE, // Don't remove or rename. Shall always be the first enumeration.
        SIMPLE_MSG, // Do not assign any value to the enumeration.
        NO_OF_MSG_TYPES // Don't remove or rename. Shall always be the last enumeration.
    };
} // Messages
```

Messages may carry data and even functions that can be executed when a message is received:

```cpp
#include <iostream>
#include <string>
#include "Message.h"

namespace Messages
{
    class DataMsg: public Message
    {
    private:
        std::string data;

    public:
        explicit DataMsg(std::string  data): Message(MessageType::DATA_MSG), data(std::move(data)) {}
        ~DataMsg() override = default;

        const std::string& getData() const {return data;}
        friend std::ostream& operator<< (std::ostream& out, const DataMessage* msg)  {return out << msg->getData();}
    }; // DataMsg
} // Messages
```

#### Operations on messages

There are three operations which can be applied on messages: This is to subscribe, unsubscribe and publish a message.

#### Subscribe to a Message

An Actor subscribes to a specific message type by providing a callback function. This function is
executed each time a message of that type is published.

##### The 'subscribe' function

```cpp
Subscription_t subId = subscribe(MessageType type, const std::function<void(Message*)>& func)

// type: A specified message type. The available messages types are defined in MessageType.h
// func: A lambda or callback function. The function must take tr as argument.
// returns: A subscription id (unsigned long)
```

##### Example

```cpp
Messenger::subscribe(MessageType::DATA_MSG, [this](Message* msg) {
    auto* dataMsg = dynamic_cast<DataMessage*>(msg.get());
    Logger::debug() << dataMsg;
});
```

or

```cpp
Messenger::subscribe(MessageType::PUB_SUB, std::bind(&Subscriber::cbFunc, this, std::placeholders::_1));

void cbFunc(Message* msg) {
    auto* dataMsg = dynamic_cast<DataMessage*>(msg);
    Logger::debug() << dataMsg;
}
```

#### Unsubscribe to a Message

An Actor can at any time unsubscribe a subscription.

##### The 'unsubscribe' function

```cpp
void unsubscribe(Subscription_t subId)

// subId: the returned value of a subscription.
```

##### Example

```cpp
subscription_t subId = Messenger::subscribe(MessageType::DATA_MSG, [this](const tr& msg) {
    auto* dataMsg = dynamic_cast<DataMessage*>(msg.get());
    Logger::debug() << dataMsg;
});

//...

Messenger::unsubscribe(subId);
```

#### Publish a Message

An Actor publishes messages by means of the publish function.

##### The 'publish' function

```cpp
void publish(Message* msg)
// msg: The message (instance of a class) to be published.
 ```

##### Example

```cpp
Messenger::publish(new DataMsg("Hello wold."));
```

The sequence diagram below shows how the subscription and publishing of messages work.
The Actor starts by subscribing to a number of messages (message types).
A callback function is associated to each subscription.
Each time a message is published the set of callback functions that have subscribed to the message will be executed.
This takes place in the Dispatcher where a number of Worker threads will take care of the execution.

<p align="center">
  <img src="https://github.com/henrik7264/Actors/blob/main/images/Actors_Publish_Subscribe.png"><br>
  Sequence diagram showing the subscribe and publish mechanism of the Actors Library.
</p>

There are some problems related to this execution model/architecture:
1. While executing one callback function another message may be published and trigger another callback function.
   This could in worse case lead to thread synchronization problems. The Actors library solves this problem by allowing
   only one callback function per Actor to be executed at a time, i.e. 100 Actors can concurrently execute 100 callback functions,
   but one Actor can only execute one callback function at a time.
2. A heavy message load may create the situation described in item 1. To accommodate for this problem,
   the Actors library makes use of a number of threads. Each message type is assigned to a dedicated queue and execution thread.

      if the messages cannot be handled as fast as they arrive. This can in worse case lead to a large amount Workers (threads).

The problems described above are common for this kind of architecture.
There is no real solution to the problem except that the architect and programmer must ensure
that the hardware platform is dimensioned to the message load of the system 
and that the callback functions are fast and responsive. 
Avoid sleep, wait and I/O operations in callback functions that are called often.

### Actors

Actors are, like messages, a central part of the Actors library. All Actors are sub-classes of the Actor class:

#### Creation of an Actor

```cpp
namespace Actors
{
    struct MyActor: public Actor
    {
        MyActor(): Actor("MY_ACTOR") {}
        ~MyActor() override = default;
    }; // MyActor
} // Actors
```

It is as simple as that! The Actor takes as argument the name of the Actor.
It must be a unique name that is easy to identify in ex. log message.
The following example shows how the MyActor subscribes to DATA_MSG messages:

#### Creation of an Actor that subscribes to DATA_MSG messages

```cpp
namespace Actors
{
    struct MyActor: public Actor
    {
        MyActor(): Actor("MY_ACTOR") {
            Messenger::subscribe(MessageType::DATA_MSG, [this](Message* msg){
                auto* dataMsg = dynamic_cast<DataMessage*>(msg);
                Logger::debug() << dataMsg;
            });
        }
        ~MyActor() override = default;
    }; // MyActor
} // Actors
```

#### Initialisation of Actors

Initialization of an Actor consist of creating an instance of it. It can be done from anywhere and at any time -
even an Actor may create new Actors.

```cpp
int main()
{
    // initialise all Actors
    auto actors = {std::shared_ptr<Actors::Actor>(new Actors::MyActor())};

    // Let the program run for 10 sec. Replace this code with your own.
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    return 0;
}
```

The initialization of the actors in the above example is a bit cumbersome -
especially if there are many actors to be initialized.
To simplify the code add the following preprocessor statement to the MyActor file:

```cpp
// Only defined to simplify initialization of actors.
#define MY_ACTOR() std::shared_ptr<Actors::Actor>(new Actors::MyActor())
```

Now it will be possible to initialize the Actors as follows:

```cpp
int main()
{
    // initialise all Actors
    auto actors = {MY_ACTOR()};
    
}
```

#### Actors provided interfaces

An Actor is implemented as a facade. As soon we are in the scope of an Actor a set of functions becomes available.
This includes:

```cpp
Messenger::scribe(...)
Messenger::unsucscribe(...)
Messenger::publish(...)
Messenger::stream(...)

Logger::debug(...)
Logger::info(...)
Logger::warning(...)
Logger::error(...)
Logger::critical(...)

Scheduler::once(...)
Scheduler::repeat(...)
Scheduler::remove(...)
Scheduler::timer(...)

Statemachine_t sm = STATEMACHINE(...)
```

Observe how the functions are organized into logical groups. This makes it very easy to understand and use them.
Only Statemachines are a bit different due to their nature.

### Logging

Logging is fundamentally a debugging facility that allows the programmer to printout information
about the Actors internal state. Five log levels are provided where each level indicates
the severity of the problem. Each loglevel returns a reference to a logging object which
actually is a std::ostringstream. This means that the well-known C++ stream insertion operator
“<<” can be used to compose the logging messages. Log messages will in the end be streamed to std::clog.

#### Logging Interface

```cpp
Logger::debug() << "Here follows a stream of data separated by <<";
Logger::info() << "Here follows a stream of data separated by <<";
Logger::warning() << "Here follows a stream of data separated by <<";
Logger::error() << "Here follows a stream of data separated by <<";
Logger::fatal() << "Here follows a stream of data separated by <<";
```

### Example

```cpp
Logger::info() << "Received a Data Message " << msg->getName();
```

This will produce the following log entry:
```
2023-01-01 23:19:49,175 MyActor INFO: Received a Data Message: Hello World
```

### Scheduler

A Scheduler can be used to execute a task (function call) at a given time.
The task can be executed once or repeated until it is removed.
A special case of the scheduler is the timer which can be started and stopped after an instance has been created.
The scheduled tasks are executed by one Worker. The Scheduler is closely related to message handling - 
in fact they work in the same way and the Actors library will behave the same way:

1. While executing one task another task may be triggered by a scheduler timeout.
   This could in worse case lead to thread synchronization problems.
   The Actors library solves this problem by allowing only one task per Actor to execute at a time,
   i.e. 100 Actors can concurrently execute 100 tasks, but one Actor can only execute one task at a time.
2. Scheduled tasks and message handling works under the same principles as described above.
   Only one task/callback function can be executed at time per Actor to avoid synchronization problems.

Again, the Actors library will do what it can to solve the load problems,
but the root cause of the problem is an insufficient hardware platform and/or
poor implementations of the tasks/callback functions.
It is in these two areas the problem should be resolved.

The scheduler interface is defined as follows:

#### Schedule a task once

A task can be executed once at a given time by the Scheduler.
The once function will return a job id that can be used to cancel/remove the scheduled job.

##### The 'once' function

```cpp
Schedulers::JobId once(std::chrono::duration<long, std::milli> msec, const std::function<void()>& func)
Schedulers::JobId once(long msec, const std::function<void()>& func)

// msec: timeout in milliseconds.
// func: call back function to be executed when the job times out.
// return: jobId - umber to indentify the scheduled job.
```

##### Example

```cpp
auto jobId = Scheduler::once(1000, [this]() { // A new job is scheduled.
    logger::debug << "Scheduled job timed out!";   
}
```

#### Schedule a repeating task

A job can be scheduled to repeat a task.
The repeat function will return a job id that can be used to cancel/remove the scheduled job.

##### The 'repeat' function

```cpp
Schedulers::JobId repeat(std::chrono::duration<long, std::milli> msec, const std::function<void()>& func)
Schedulers::JobId repeat(long msec, const std::function<void()>& func)

// msec: timeout in milliseconds.
// func: call back function to be executed when the job times out.
// return: jobId - number to indentify the scheduled job.
```

##### Example

```cpp
auto jobId = Scheduler::repeat(1000, [this]() { // A new job is scheduled.
    logger::debug << "Scheduled job timed out!";   
}
```

#### Remove a scheduled job

A scheduled job can at any time be canceled/removed.

##### The 'remove' function

```cpp
void remove(Schedulers::JobId id)

// id: job to be removed.
```

##### Example

```cpp
auto jobId = Scheduler::repeat(1000, [this]() { // A new job is scheduled.
    logger::debug << "Scheduled job timed out!"    
}
// ...
Scheduler::remove(jobId)  // The job is canceled and removed.
```

### Timers

Timers are similar to schedulers, except a timer must be started before it is activated.
A timer can at anytime be stopped or restarted if needed. The timer has a timeout and a callback function.
The timer is activated when it is started, and when it times out the callback function will be executed.

#### Create a timer

A timer is created by calling the timer function of the scheduler.
It takes a timeout time and a callback function as argument.
The Timer is activated at the moment it is started.

```cpp
Timer_t timer(std::chrono::duration<long, std::milli> msec, const std::function<void()>& func)
Timer_t timer(long msec, const std::function<void()>& func)

// msec: timeout in milliseconds.
// func: call back function to be executed when the job times out.
// return: Timer_t - an instance of a Timer_t which is a shared_ptr that will be auto deleted when the instance no longer is reachable.
```

##### Example

```cpp
Timer_t t1 = scheduler::timer(1000, [this]() { // A new timer is created.
    logger::debug << "Timer t1 timed out!"
}
//...
t1->start()  // Start the timer. It will timeout 1000ms from this moment.
//...
t1->stop()  // Stop the timer.
// ...
t1->start() // Restart the timer.  It will timeout 1000ms from this moment.
```

#### Start a timer

A Timer is activated at the moment it is started. 
If needed it can at any time be restarted by calling the start function.

##### The 'start' function

```cpp
void start()
```

##### Example
```cpp
Timer_t t1 = scheduler::timer(1000, [this]() { // A new timer is created.
    logger::debug << "Timer t1 timed out!"
}
//...
t1.start()  // Start the timer. It will timeout 1000ms from this moment.
```

#### Stop a timer

A timer can at any time be stopped by calling the stop function.
The stop function will inactivate the timer and preventing it from timing out.

##### The 'stop' function

```cpp
void stop()
```

##### Example

```cpp
Timer_t t1 = scheduler::timer(1000, [this]() { // A new timer is created.
    logger::debug << "Timer t1 timed out!"
}
//...
t1.start()  // Start the timer. It will timeout 1000ms from this moment.
//...
t1.stop()  // Stop the timer.
```

### State Machines

State machines comes in many forms and is always hard to understand.
The state machine is in itself not that hard to understand:
It can be in one of a number of states and it will initially be in a predefined state.
The state machine can change from one state to another in response to an event.
The change from one state to another is called a transition.
An state machine is in other words defined by list of states, an initial state,
and the events that trigger the transitions.

#### Creating a State Machine

A state machine is created, not surprisingly, as an instance of a Statemachine. 
To simplify the definition or writing of a state machine,
macros named STATEMACHINE, STATE, MESSAGE, TIMER and NEXT_STATE have been defined.
A general template for a state machine is shown here:

```cpp
Statemachine_t sm = STATEMACHINE(initial_state,
            STATE(state1, ...),
            STATE(state2, ...)
            ...
            STATE(stateN, ...))
```

It takes as argument an initial state and a number of states. Each state is identified by a unique state id.
It can be a number or enumeration. Use enumerations as shown in the example below.
This is a nice way to define state ids.

#### Example

The following example shows the definition of a state machine of a door.
The door can be in state DOOR_CLOSED or DOOR_OPENED. The door is initially in state DOOR_CLOSED.

```cpp
enum State {DOOR_OPENED, DOOR_CLOSED};
StateMachine_t sm = STATEMACHINE(State::DOOR_CLOSED,
             STATE(State::DOOR_CLOSED, ...),
             STATE(State::DOOR_OPENED, ...));
```

#### Creating a State

A State is defined by its id and a number of transitions that each is triggered by an event.

```cpp
enum State {STATE_A, STATE_N};
StateMachine_t sm = STATEMACHINE(STATE_A,
             STATE(Starte::STATE_A,
                   TRANSITION(...),
                   ...
                   TRANSITION(...)),
             STATE(State::STATE_N, ...)
                   TRANSITION(...),
                   ...
                   TRANSITION(...)));
```

Two types of transitions can be used. That is the MESSAGE transition
which is triggered by a Message that is published by an Actor, and the TIMER transition
that is triggered by a timer timeout.

#### Example

```cpp
enum State {DOOR_OPENED, DOOR_CLOSED};
StateMachine_t sm = STATEMACHINE(DOOR_CLOSED,
             STATE(State::DOOR_CLOSED,
                   MESSAGE(MessageType_t::OPEN_DOOR, ...)),
             STATE(State::DOOR_OPENED,
                    MESSAGE(MessageType_t::CLOSE_DOOR, ...),
                    TIMER(1000, ...)));
```

#### Creating a Transition

To get the full picture of the state machine's capabilities we have to understand transitions.
A transition is triggered by an event. When it is triggered it will perform an action (optional),
and finally it will change to a new/next state (optional).

```cpp
enum State {DOOR_OPENED, DOOR_CLOSED};
StateMachine_t sm = STATEMACHINE(State::DOOR_CLOSED,
             STATE(State::DOOR_CLOSED,
                   MESSAGE(MessageType_t::OPEN_DOOR, NEXT_STATE(State::DOOR_OPENED), [this](Message* msg){
                       Logger::debug() << "Opening door ...";})),
             STATE(State::DOOR_OPENED,
                   MESSAGE(MessageType_t::CLOSE_DOOR, NEXT_STATE(State::DOOR_CLOSED), [this](Message* msg){
                       Logger::debug() << "Closing door ...";}),
                   TIMER(1000, NEXT_STATE(State::DOOR_CLOSED), [this](){
                       Logger::debug() << "Auto closing door ...";})));

```

Two types of transitions exists. That is the MESSAGE transition which is triggered by a Message event
published by an Actor, and the TIMER transition that is triggered by a timer timeout.

The action part is simply a function or lambda expression that is executed as part handling the transition.
When the operation is complete the transition will set the new/next state of the state machine.
Observe that both the action and next_state are optional.

#### Observations related to State Machines

The state machine is a bit tricky, but care has been taken to make it simple and safe to use:

1. A state machine is tightly coupled to an Actor. In fact, the state machine can not be used outside the scope of an Actor.
2. A transition is an "atomic" operation, but with some restrictions. It works as follows:
    1. A message is published by an Actor.
    2. The state machine will check if it has a transition that is triggered by the message event.
    3. If this is the case, the state machine will be locked until the transition has been executed.
    4. A dedicated Worker thread will first execute the action and then set the next state of the state machine.
    5. The state machine is unlocked and new events can be handled.

   If a timer timeout during the execution of a transition it will be dropped.
   An incoming Message event will be postponed if a transaction is being executed.
3. An Actor can define more state machines. The state machines are updated concurrently
   and there is no way to determine if one state machine is updated before the other.
4. The state machine can slow down due to constraints with execution of transitions.
5. The implementation of the state machine is complex and needs to be refactored and optimized.

### Message Streams
