cmake_minimum_required(VERSION 3.16)

# project name
project(Glad LANGUAGES C)

# create static library
add_library(glad STATIC include/glad/glad.h src/glad.c)
target_include_directories(glad PUBLIC include/)