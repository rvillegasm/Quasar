cmake_minimum_required(VERSION 3.16)

# set the project name
project(sandbox)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "ERROR: In-source builds are not allowed!. 
  You should create a separate directory for build files.")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# add the executable
add_executable(sandbox src/SandboxApp.cpp)

# include directories
target_include_directories(sandbox PRIVATE
  "${CMAKE_SOURCE_DIR}/../Quasar/src"
  "${CMAKE_SOURCE_DIR}/../Quasar/vendor/spdlog/include"
  "${CMAKE_SOURCE_DIR}/../Quasar/vendor/glm"
)

# link libraries
target_link_libraries(sandbox PRIVATE
  "${CMAKE_SOURCE_DIR}/../Quasar/build/lib/libquasar.so"
)