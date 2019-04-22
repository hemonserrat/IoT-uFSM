cmake_minimum_required(VERSION 3.9)
project(cppfsm)

set(CMAKE_CXX_STANDARD 11)

include_directories(inc)

add_executable(cppfsm
        inc/uFsm.hpp
        inc/uEventHandler.hpp
        src/uFsm.cpp
        src/uEventHandler.cpp
        CFsmTest.cpp)
