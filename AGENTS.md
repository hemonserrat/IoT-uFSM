# Repository Instructions

## Project Overview

IoT-uFSM is a small C++11 event-driven finite state machine library. The public
API is intentionally compact:

- `uFsm` owns the transition table, queues events, and drives state changes.
- `uEventHandler` is the base class for user-defined transition handlers.
- `uFsmEvent` stores an event id and an optional `void *` parameter pointer.

Preserve the low-dependency, embedded-friendly shape of the library. Avoid
introducing runtime dependencies outside the C++ standard library unless the
user explicitly asks for them.

## Repository Layout

- `inc/`: public headers. Keep declarations here.
- `src/`: implementations for the library. Keep definitions here.
- `test_fsm.cpp`: Catch2 unit tests.
- `CFsmTest.cpp`: example/demo executable.
- `CMakeLists.txt`: build, test, and install configuration.
- `Doxyfile`: Doxygen configuration.
- `docs/`: generated Doxygen HTML. Do not edit generated files by hand unless
  the task is specifically about committed generated documentation.
- `.github/workflows/ci.yml`: CI build, test, and analysis workflow.
- `sonar-project.properties`: SonarCloud configuration.

## Coding Rules

- Use C++11 only. `CMakeLists.txt` sets `CMAKE_CXX_STANDARD 11`; do not use
  newer language or library features.
- Before adding a new FSM-related class, inspect `inc/` and `src/` for existing
  extension points and naming patterns.
- Keep public declarations in `inc/` and implementation details in `src/`.
- Follow the existing core naming convention with the `u` prefix, such as
  `uFsm`, `uEventHandler`, and `uFsmEvent`.
- Keep the library event-driven: new behavior should fit the transition table,
  event queue, and handler callback model.
- Preserve caller ownership of event parameter data. `uFsmEvent::parameters_`
  is a non-owning `void *`; do not delete or retain pointed-to data unless an
  API change explicitly defines ownership.
- Use RAII for new resources. Prefer standard containers or smart ownership
  where compatible with the existing C++11 API, and avoid raw `new`/`delete`
  in new code unless matching an existing boundary.
- Keep changes small and compatible with embedded use cases: no exceptions,
  threading, logging frameworks, or heavyweight abstractions without a clear
  requirement.
- Maintain the surrounding style manually. There is no project formatter or
  custom linter.

## Documentation

- Add Doxygen comments for every public class and public method.
- Match the existing block style:

```cpp
/**
 * Brief description.
 *
 * \param name parameter description.
 * \return return value description.
 */
```

- Include `\file` comments in new public headers and implementation files.
- Document pointer ownership and lifetime when adding parameters that accept or
  return raw pointers.

## Build And Test

Use the normal CMake workflow from the repository root:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

Catch2 v3.6.0 is fetched by CMake `FetchContent`; no separate package manager is
required. If network access is unavailable, report that dependency fetching was
blocked instead of vendoring dependencies.

When changing behavior, add or update tests in `test_fsm.cpp`. Cover both the
successful transition path and relevant error or boundary cases, especially:

- missing or internal-error transitions,
- handler return values,
- event parameter passing,
- transition table capacity and lookup behavior,
- queued events generated from handlers.

## Pull Requests

When preparing PR notes, include:

- a concise summary of the user-visible or API behavior changed,
- tests run, including any failures or skipped checks,
- documentation updates, if public APIs changed,
- compatibility notes for C++11 or existing consumers when relevant.

Do not include generated `docs/` changes unless documentation generation was
part of the requested work.
