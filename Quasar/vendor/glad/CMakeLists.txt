cmake_minimum_required(VERSION 3.16)

# project name
project(Glad LANGUAGES C)

# create static library
add_library(glad include/glad/glad.h src/glad.c)
target_include_directories(glad PUBLIC include/)

# compiler flags
# -fPIC to allow position independent code when using this lib to build a shared object
target_compile_options(glad PRIVATE -fPIC)