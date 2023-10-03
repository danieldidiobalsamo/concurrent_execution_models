cmake_minimum_required(VERSION 3.20)

project(concurrentModels)

find_package( Boost 1.74.0 COMPONENTS thread fiber context )

# C++ standard and compilation flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra -Wold-style-cast -Wno-unused-command-line-argument")

set(SRC
    inc/ProducerConsumer.hpp
    src/ProducerConsumer.cpp

    src/main.cpp)

include_directories(inc ${Boost_INCLUDE_DIRS})

add_executable(concurrentModels ${SRC})
target_link_libraries(concurrentModels PUBLIC ${Boost_LIBRARIES})
