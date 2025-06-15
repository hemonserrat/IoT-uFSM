# IoT-uFSM
[![Build Status](https://github.com/hemonserrat/IoT-uFSM/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/hemonserrat/IoT-uFSM/actions/workflows/ci.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=hemonserrat_IoT-uFSM&metric=alert_status)](https://sonarcloud.io/dashboard?id=hemonserrat_IoT-uFSM)

[IoT u-Micro Finite State Machine](https://hemonserrat.github.io/IoT-uFSM/index.html)

## Build Instructions (macOS)

1. Install CMake (if not already installed):
   ```bash
   brew install cmake
   ```
2. Create a build directory:
   ```bash
   mkdir -p build
   ```
3. Run CMake to configure the project:
   ```bash
   cmake -S . -B build
   ```
4. Build the project:
   ```bash
   cmake --build build
   ```

The binaries will be generated in the `build` directory.



