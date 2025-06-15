cmake_minimum_required(VERSION 3.10)
project(cppfsm)

set(CMAKE_CXX_STANDARD 11)

include_directories(inc)
include_directories(/usr/local/include)


add_executable(cppfsm
        inc/uFsm.hpp
        inc/uEventHandler.hpp
        src/uFsm.cpp
        src/uEventHandler.cpp
        CFsmTest.cpp)

# Add Catch2 via FetchContent
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.6.0
)
FetchContent_MakeAvailable(Catch2)

# find_package(Catch2 3 REQUIRED)

# Enable testing and Catch2 test discovery
include(CTest)
include(Catch)

enable_testing()

add_executable(test_fsm test_fsm.cpp src/uFsm.cpp src/uEventHandler.cpp)
target_include_directories(test_fsm PRIVATE inc)
target_link_libraries(test_fsm PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(test_fsm)
