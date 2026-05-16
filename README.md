# IoT-uFSM

IoT-uFSM is a small event-driven finite state machine library for C++11
projects.  It is intended for IoT, embedded, automation, and other systems
where state transitions should remain explicit and cheap to run.

[![Build Status](https://github.com/hemonserrat/IoT-uFSM/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/hemonserrat/IoT-uFSM/actions/workflows/ci.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=hemonserrat_IoT-uFSM&metric=alert_status)](https://sonarcloud.io/dashboard?id=hemonserrat_IoT-uFSM)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![C++11](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B11)

## Features

- C++11 implementation with no runtime dependency outside the standard library.
- Event handlers are separated from the finite state machine.
- Transitions are stored in a fixed-size table chosen by the caller.
- Event parameters can be passed through as `void *` for low-level integration.
- Catch2 tests are provided for transition and handler behavior.
- Doxygen documentation is available in `docs/`.

## Build

Requirements:

- CMake 3.10 or newer.
- A C++11 compiler.

Build and test from a clean checkout:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

The default build creates:

- `IoT-uFSM`: the library target.
- `cppfsm`: the example program in `CFsmTest.cpp`.
- `test_fsm`: the Catch2 test executable.

## Use In Another CMake Project

The project can be consumed with CMake `FetchContent`:

```cmake
include(FetchContent)

FetchContent_Declare(
  IoT-uFSM
  GIT_REPOSITORY https://github.com/hemonserrat/IoT-uFSM.git
  GIT_TAG        master
)

FetchContent_MakeAvailable(IoT-uFSM)

target_link_libraries(your_target PRIVATE IoT-uFSM)
```

For small embedded builds, copying `inc/` and `src/` into a firmware tree is
also reasonable.

## Example

```cpp
#include <cstdint>
#include <iostream>

#include "uEventHandler.hpp"
#include "uFsm.hpp"

#define IDLE_STATE     0x0001
#define ACTIVE_STATE   0x0002
#define ERROR_STATE    0xffff

#define START_EVENT    0x0001
#define STOP_EVENT     0x0002
#define ERROR_EVENT    0x0003

enum HandlerIndex {
    StartIndex,
    StopIndex,
    ErrorIndex,
};

class DeviceHandler : public uEventHandler {
public:
    DeviceHandler() : uEventHandler(3) {
        FillHandlersArray();
    }

private:
    void FillHandlersArray(void) override {
        functions_[StartIndex] = (TransitionFunc)&DeviceHandler::start;
        functions_[StopIndex] = (TransitionFunc)&DeviceHandler::stop;
        functions_[ErrorIndex] = (TransitionFunc)&DeviceHandler::fail;
    }

    bool start(void *) {
        std::cout << "start\n";
        return true;
    }

    bool stop(void *) {
        std::cout << "stop\n";
        return true;
    }

    bool fail(void *) {
        std::cout << "error\n";
        return false;
    }
};

int main(void) {
    DeviceHandler handler;
    uFsm fsm(&handler, 10, IDLE_STATE);

    fsm.defineTransition(IDLE_STATE, ACTIVE_STATE, START_EVENT, StartIndex);
    fsm.defineTransition(ACTIVE_STATE, IDLE_STATE, STOP_EVENT, StopIndex);
    fsm.defineTransition(IDLE_STATE, ERROR_STATE, ERROR_EVENT, ErrorIndex);

    fsm.control(START_EVENT);
    fsm.control(STOP_EVENT);

    return 0;
}
```

## API Shape

The library has two main classes:

- `uFsm` owns the transition table and processes queued events.
- `uEventHandler` is the base class used to bind transition handlers.

`uFsmEvent` stores an event id and an optional parameter pointer.  The caller
owns the pointed-to data and must manage its lifetime.

Generated API documentation can be opened from `docs/index.html`.

## Development

Read `CONTRIBUTING.md` before sending a patch.  The short version is:

- Keep changes small and reviewable.
- Preserve C++11 compatibility.
- Add tests for behavioral changes.
- Document public APIs with Doxygen comments.
- Run `cmake --build build` and `ctest --test-dir build` before submitting.

## License

IoT-uFSM is free software released under the GNU General Public License,
version 3 or later.  See `LICENSE` for the full license text.
