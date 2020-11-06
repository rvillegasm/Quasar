cmake_minimum_required(VERSION 3.16)

# set the project name
project(sandbox)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "ERROR: In-source builds are not allowed!. 
  You should create a separate directory for build files.")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# set the quasar dependencies dir in order to compile and link against them
set(QUASAR_DEPENDENCIES_DIR "${CMAKE_SOURCE_DIR}/../Quasar/vendor")
# set the quasar lib directory in order to link against it
set(QUASAR_LIB_DIR "${CMAKE_SOURCE_DIR}/../Quasar/build/lib")

# add the executable
add_executable(sandbox src/SandboxApp.cpp)

# include directories
target_include_directories(sandbox PRIVATE
  "${CMAKE_SOURCE_DIR}/../Quasar/src"
  "${QUASAR_DEPENDENCIES_DIR}/spdlog/include"
  "${QUASAR_DEPENDENCIES_DIR}/glm"
  "${QUASAR_DEPENDENCIES_DIR}/imgui"
)

# ------------------------------------
# ----- Third party dependencies -----
# ------------------------------------

# GLFW (and OpenGL)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${QUASAR_DEPENDENCIES_DIR}/GLFW lib/GLFW)

# glad
add_subdirectory(${QUASAR_DEPENDENCIES_DIR}/glad lib/glad)

# ImGui
set(IMGUI_SOURCES
  ${QUASAR_DEPENDENCIES_DIR}/imgui/imgui.cpp
  ${QUASAR_DEPENDENCIES_DIR}/imgui/imgui_widgets.cpp
  ${QUASAR_DEPENDENCIES_DIR}/imgui/imgui_draw.cpp
  ${QUASAR_DEPENDENCIES_DIR}/imgui/imgui_demo.cpp
  ${QUASAR_DEPENDENCIES_DIR}/imgui/backends/imgui_impl_glfw.cpp
  ${QUASAR_DEPENDENCIES_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC 
  ${QUASAR_DEPENDENCIES_DIR}/imgui 
  ${QUASAR_DEPENDENCIES_DIR}/GLFW/include 
  ${QUASAR_DEPENDENCIES_DIR}/glad/include
)

# link every library
target_link_libraries(sandbox PRIVATE 
  "${QUASAR_LIB_DIR}/libquasar.a"
  imgui
  glfw
  glad
)

# ------------------------------------
# --------------- END ----------------
# ------------------------------------

# compile definitions
target_compile_definitions(sandbox PRIVATE
  GLFW_INCLUDE_NONE # do not include opengl with glfw, since glad already does that
)
# show warnings on debug build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(sandbox PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
      -Wall>
    $<$<CXX_COMPILER_ID:MSVC>:
      /W4>)
endif()