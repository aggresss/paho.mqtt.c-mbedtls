# ------------------------------------------------------------------------------
# CMakeLists.txt for aggresss/paho.mqtt.c-mbedtls
# ------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake" ${CMAKE_MODULE_PATH})
include(Utilities)
include(GNUInstallDirs)

# ------------------------------------------------------------------------------
# Cross compile
# ------------------------------------------------------------------------------

# MUST before project()
if(CROSS_COMPILE)
  set(CMAKE_C_COMPILER ${CROSS_COMPILE}-gcc)
  set(CMAKE_CXX_COMPILER ${CROSS_COMPILE}-g++)
  set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
  set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
  set(TOOLCHAIN_FILE "${CMAKE_CURRENT_BINARY_DIR}/toolchain.cmake")
  file(WRITE ${TOOLCHAIN_FILE}
"# ------------------------------------------------------------------------------
# toolchain.cmake
# ------------------------------------------------------------------------------

# this is required
set(CMAKE_SYSTEM_NAME Linux)

# specify the cross compiler
set(CMAKE_C_COMPILER ${CROSS_COMPILE}-gcc)
set(CMAKE_CXX_COMPILER ${CROSS_COMPILE}-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
\n"
  )
endif()

# ------------------------------------------------------------------------------
# Project
# ------------------------------------------------------------------------------

project("paho.mqtt.c-mbedtls")

# ------------------------------------------------------------------------------
# Get and Display infomation
# ------------------------------------------------------------------------------

message(STATUS "CMake version: " ${CMAKE_VERSION})
message(STATUS "CMake system name: " ${CMAKE_SYSTEM_NAME})

c_compiler_prefix(C_COMPILER_PREFIX)
if(C_COMPILER_PREFIX)
  message(STATUS "C compiler prefix: " ${C_COMPILER_PREFIX})
endif()

c_compiler_machine(C_COMPILER_MACHINE)
if(C_COMPILER_MACHINE)
  message(STATUS "C compiler machine infomation: " ${C_COMPILER_MACHINE})
endif()

# ------------------------------------------------------------------------------
# Build type
# ------------------------------------------------------------------------------

set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}
  CACHE STRING "Choose the type of build: None Debug Release Coverage" FORCE)

if(CMAKE_BUILD_TYPE)
  message(STATUS "CMake build type: " ${CMAKE_BUILD_TYPE})
endif()

# Default C / CXX flags
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
  set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -ggdb -O0 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -Wall -DNDEBUG -DRELEASE")
set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})

set(CMAKE_C_FLAGS_COVERAGE "${CMAKE_C_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS_COVERAGE ${CMAKE_C_FLAGS_COVERAGE})

# ------------------------------------------------------------------------------
# Test
# ------------------------------------------------------------------------------

find_program(VALGRIND valgrind)
if(VALGRIND)
  # Ctest configuration variables must be set BEFORE include(Ctest)
  set(MEMORYCHECK_COMMAND ${VALGRIND})
  # Check process children and give detail for each leak
  set(MEMORYCHECK_COMMAND_OPTIONS
    "${MEMORYCHECK_COMMAND_OPTIONS} --leak-check=yes --show-reachable=yes")
  set(MEMORYCHECK_COMMAND_OPTIONS
    "${MEMORYCHECK_COMMAND_OPTIONS} --trace-children=yes --track-fds=yes")
  # Set error code.
  set(MEMORYCHECK_COMMAND_OPTIONS
    "${MEMORYCHECK_COMMAND_OPTIONS} --error-exitcode=255")
  # Generate suppression hint.
  set(MEMORYCHECK_COMMAND_OPTIONS
    "${MEMORYCHECK_COMMAND_OPTIONS} --gen-suppressions=all")
  # Set suppression file
  set(MEMORYCHECK_COMMAND_OPTIONS
    "${MEMORYCHECK_COMMAND_OPTIONS} --suppressions=${CMAKE_CURRENT_LIST_DIR}/valgrind.supp")
endif()
include (CTest)
enable_testing()

# ------------------------------------------------------------------------------
# Project options
# ------------------------------------------------------------------------------

option(BUILD_ALL "Build all submodule" OFF)

# ------------------------------------------------------------------------------
# Common directory
# ------------------------------------------------------------------------------

set(TOP_DIRS ${PROJECT_SOURCE_DIR})
set(CONFIG_DIRS ${CMAKE_CURRENT_BINARY_DIR})
set(THIRD_PARTY_DIRS ${PROJECT_SOURCE_DIR}/third_party)

# ------------------------------------------------------------------------------
# Configuration file
# ------------------------------------------------------------------------------

configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h"
    @ONLY)

# ------------------------------------------------------------------------------
# Subdirectory
# ------------------------------------------------------------------------------

add_subdirectory(third_party)
add_subdirectory(examples)

