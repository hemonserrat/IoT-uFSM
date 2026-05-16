# Contributing

Thanks for taking the time to improve IoT-uFSM.  The project is small, so the
process should stay small too: clear issues, focused patches, and tests that
show the intended behavior.

## Before You Start

- Open an issue for larger changes before writing code.
- For small fixes, a pull request with a clear explanation is enough.
- Keep unrelated cleanups out of feature patches.
- Prefer simple C++11 over newer language features.

## Development Setup

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

Catch2 is fetched by CMake during configuration.

## Patch Style

- Follow the existing `inc/` and `src/` split.
- Use the existing `u` prefix for core classes.
- Keep public interfaces documented with Doxygen comments.
- Keep ownership rules explicit.  Prefer RAII where the API permits it.
- Avoid broad rewrites unless they are needed for the change at hand.
- Write commit messages in the imperative mood, for example:
  `Add bounds check for transition table`.

## Tests

Add or update tests in `test_fsm.cpp` for behavior changes.  Tests should cover
the successful path and the failure or boundary condition that matters.

Run this before opening a pull request:

```sh
cmake --build build
ctest --test-dir build
```

## Pull Requests

A good pull request includes:

- A short description of the problem.
- A short description of the fix.
- Notes about compatibility or API changes, if any.
- The test command you ran.

The maintainer may ask for changes.  That is normal review, not a rejection.
