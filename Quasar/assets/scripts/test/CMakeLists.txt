cmake_minimum_required(VERSION 3.19)
project(test_script)

set(CMAKE_CXX_STANDARD 20)

add_library(test_script SHARED TestScript.cpp TestScript.h)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../ ${CMAKE_CURRENT_BINARY_DIR}/Quasar)

target_include_directories(test_script PRIVATE ../../../src)
target_include_directories(test_script PRIVATE ../../../vendor/glm)
target_include_directories(test_script PRIVATE ../../../vendor/spdlog/include)
target_include_directories(test_script PRIVATE ../../../vendor/entt/src)