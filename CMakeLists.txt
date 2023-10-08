cmake_minimum_required(VERSION 3.5)
project(RTLite)

# Platform detection and messaging
if(WIN32)
  if(CYGWIN)
    message("CYGWIN Not Supported for RTLite")
  else()
    message("WIN32 for RTLite")
  endif()
else()
  if(UNIX)
    if(APPLE)
      message("APPLE for RTLite")
    else()
      message("UNIX for RTLite")
    endif()
  else()
    message(SEND_ERROR "Unable to determine platform")
  endif()
endif()

# Determine build type and set flags accordingly
get_cmake_property(CACHE_VARS CACHE_VARIABLES)
foreach(CACHE_VAR ${CACHE_VARS})
  if(${CACHE_VAR} STREQUAL CMAKE_BUILD_TYPE)
    if(${${CACHE_VAR}} STREQUAL "Debug")
      set(FORCE_DEBUG "Debug")
    elseif(${${CACHE_VAR}} STREQUAL "MinSizeRel")
      set(FORCE_DEBUG "MinSizeRel")
    endif()
  endif()
  get_property(CACHE_VAR_HELPSTRING CACHE ${CACHE_VAR} PROPERTY HELPSTRING)
  if(CACHE_VAR_HELPSTRING STREQUAL "No help, variable specified on the command line.")
    get_property(CACHE_VAR_TYPE CACHE ${CACHE_VAR} PROPERTY TYPE)
    if(CACHE_VAR_TYPE STREQUAL "UNINITIALIZED")
      set(CACHE_VAR_TYPE)
    else()
      set(CACHE_VAR_TYPE :${CACHE_VAR_TYPE})
    endif()
    set(CMAKE_ARGS "${CMAKE_ARGS} -D${CACHE_VAR}${CACHE_VAR_TYPE}=\"${${CACHE_VAR}}\"")
  endif()
endforeach()

# Debug flag handling
if(FORCE_DEBUG)
  set(CMAKE_BUILD_TYPE "${FORCE_DEBUG}" CACHE STRING "Default build type: ${FORCE_DEBUG}" FORCE)
endif()

# Compiler-specific flags and standards
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-show-option -Wall -Wextra -std=c++2a")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-show-option -Wall -Wextra -std=c++2a")
elseif(MSVC)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  add_definitions(-DBOOST_USE_WINAPI_VERSION=BOOST_WINAPI_VERSION_WIN7)
endif()

# Boost library configuration
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)
if(FORCE_DEBUG)
  set(Boost_DEBUG ON)
else()
  set(Boost_DEBUG OFF)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

if (WIN32)
  include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
  conan_basic_setup()
else()
  find_package(Boost)
  find_package(Threads)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

# OpenSSL library configuration
set(OPENSSL_USE_STATIC_LIBS ON)
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# Compiler-specific adjustments
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
endif()

# Set up preprocessor macros and flags for Windows
if (WIN32)
  add_definitions(-D_WIN32_WINNT=0x0601)
  # Additional Windows-specific settings can be added here if needed.
endif()

# Installation directory based on build type
if(FORCE_DEBUG)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_HOME_DIRECTORY}/${FORCE_DEBUG}" CACHE PATH "Default install path" FORCE)
else()
  set(CMAKE_INSTALL_PREFIX "${CMAKE_HOME_DIRECTORY}/release" CACHE PATH "Default install path" FORCE)
endif()

# Add an executable for the test suite
add_executable(MyTestSuite main.cpp)

# Link the test executable with Boost.Test
target_link_libraries(MyTestSuite Boost::unit_test_framework)

# Set the working directory for running tests
set_target_properties(MyTestSuite PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set up the tests as a part of the build process
enable_testing()

# Add the test executable as a test
add_test(NAME MyTestSuite COMMAND MyTestSuite)

# Create a custom command to run the tests
add_custom_command(
    TARGET MyTestSuite
    POST_BUILD
    COMMAND MyTestSuite --log_level=test_suite
    COMMENT "Running tests..."
    VERBATIM
)

# List and print all CMake cache variables
get_cmake_property(CACHE_VARS_TO_PRINT CACHE_VARIABLES)
foreach(CACHE_VAR ${CACHE_VARS_TO_PRINT})
  message("CACHE_VAR => ${CACHE_VAR} ${${CACHE_VAR}}")
endforeach()
