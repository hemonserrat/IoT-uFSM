cmake_minimum_required(VERSION 3.10)
project(cppfsm)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(IoT-uFSM
        src/uFsm.cpp
        src/uEventHandler.cpp)

target_include_directories(IoT-uFSM
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:include>)

add_executable(cppfsm
        CFsmTest.cpp)

target_link_libraries(cppfsm PRIVATE IoT-uFSM)

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

add_executable(test_fsm test_fsm.cpp)
target_link_libraries(test_fsm PRIVATE IoT-uFSM Catch2::Catch2WithMain)
catch_discover_tests(test_fsm)

install(TARGETS IoT-uFSM
        EXPORT IoT-uFSMTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(DIRECTORY inc/ DESTINATION include)
