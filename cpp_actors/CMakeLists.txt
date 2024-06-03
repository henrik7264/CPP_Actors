cmake_minimum_required(VERSION 3.6)
project(CPP_Actors)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(lib_messages lib_actors)

add_executable(example_pubsub examples/publisher_subscriber/main.cpp)
add_executable(example_smachine examples/statemachine/main.cpp)
add_executable(example_pubsub10 examples/publisher_subscriber10/main.cpp)
