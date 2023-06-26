cmake_minimum_required(VERSION 3.22)
project(cpp_actors)

set(CMAKE_CXX_STANDARD 17)

include_directories(lib_messages lib_actors)

add_executable(example_pubsub example_publisher_subscriber/main.cpp)
add_executable(example_smachine example_statemachine/main.cpp)
