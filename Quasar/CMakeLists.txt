cmake_minimum_required(VERSION 3.16)

# project name
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
set(SOURCE_DIR "src")

# Source files
set(SOURCES 
  ${SOURCE_DIR}/Quasar/Application.cpp
  ${SOURCE_DIR}/Quasar/Log.cpp
  ${SOURCE_DIR}/Platform/Linux/LinuxWindow.cpp
)

# Add the library
add_library(quasar SHARED ${SOURCES})

# Set public API
set_target_properties(quasar PROPERTIES PUBLIC_HEADER src/Quasar.h)

# Top level include directories
target_include_directories(quasar PRIVATE src)

# ----- Third party dependencies -----

# spdlog
target_include_directories(quasar PUBLIC vendor/spdlog/include)

# GLFW (and OpenGL)
find_package(OpenGL REQUIRED)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/GLFW)
target_link_libraries(quasar glfw OpenGL::GL)

# ------------------------------------

# compile definitions
target_compile_definitions(quasar PRIVATE QS_ENABLE_ASSERTS)