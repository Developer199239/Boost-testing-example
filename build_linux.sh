#!/bin/bash

# 1. Remove the build folder (if it exists) and CMakeUserPresets.json
rm -rf build CMakeUserPresets.json
# 1. Detect the Conan profile (if not already set)
# conan profile detect --force

# 2. Install dependencies using Conan
mkdir build && cd build

conan install .. --output-folder=. --build=missing -s build_type=Debug
cmake -DCMAKE_CONFIGURATION_TYPES=Debug -DCMAKE_BUILD_TYPE=Debug .. -G "Unix Makefiles"

# 5. Build the project using CMake
cmake --build .

