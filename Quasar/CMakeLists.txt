cmake_minimum_required(VERSION 3.16)

# set the project name
project(Quasar)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "ERROR: In-source builds are not allowed!. 
  You should create a separate directory for build files.")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

# Source files
set(SOURCES 
  ${SOURCE_DIR}/Quasar/Application.cpp
)

# Add the library
add_library(quasar SHARED ${SOURCES})

# Set public API
set_target_properties(quasar PROPERTIES PUBLIC_HEADER include/Quasar.h)

# Top level include directories
target_include_directories(quasar PRIVATE include)
target_include_directories(quasar PRIVATE src)