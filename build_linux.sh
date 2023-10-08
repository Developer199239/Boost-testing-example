#!/bin/bash

# 1. Remove the build folder (if it exists) and CMakeUserPresets.json
rm -rf build CMakeUserPresets.json
# 1. Detect the Conan profile (if not already set)
# conan profile detect --force

# 2. Install dependencies using Conan
mkdir build && cd build

conan install .. --output-folder=. --build=missing -s build_type=Debug

# conan install . --profile=default --output-folder=build --build=missing

# mkdir build

# 3. Change directory to the build folder
# cd build

# 4. Configure the project using CMake
# cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake -DCMAKE_CONFIGURATION_TYPES=Debug -DCMAKE_BUILD_TYPE=Debug .. -G "Unix Makefiles"
# cmake -DCMAKE_CONFIGURATION_TYPES=Debug -DCMAKE_BUILD_TYPE=Debug .. -G "Xcode"

# 5. Build the project using CMake
cmake --build .
# cmake --build . --config Debug

# chmod +x build_linux.sh

