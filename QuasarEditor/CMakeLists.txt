cmake_minimum_required(VERSION 3.16)

# set the project name
project(quasar-editor)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "ERROR: In-source builds are not allowed!. You should create a separate directory for build files.")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# set the quasar dir in order to compile and link against it
set(QUASAR_DIR "${CMAKE_SOURCE_DIR}/../Quasar")
set(SOURCE_DIR "src")

# source files
set(SOURCES
  ${SOURCE_DIR}/QuasarEditorApp.cpp
  ${SOURCE_DIR}/EditorLayer.cpp
  ${SOURCE_DIR}/Panels/SceneHierarchyPanel.cpp
  ${SOURCE_DIR}/Panels/ContentBrowserPanel.cpp
)

# add the executable
add_executable(quasar-editor ${SOURCES})

# ------------------------
# ----- Dependencies -----
# ------------------------
add_subdirectory(${QUASAR_DIR} lib/Quasar)

# include directories
target_include_directories(quasar-editor PRIVATE
  "${CMAKE_SOURCE_DIR}/src"
  "${QUASAR_DIR}/src"
  "${QUASAR_DIR}/vendor/spdlog/include"
  "${QUASAR_DIR}/vendor/glm"
  "${QUASAR_DIR}/vendor/imgui"
  "${QUASAR_DIR}/vendor/entt/src"
  "${QUASAR_DIR}/vendor/ImGuizmo"
)

# link every library
target_link_libraries(quasar-editor PRIVATE 
  quasar
  imgui
  glfw
  glad
  yaml-cpp
)
# ----------------------------
# ----- END Dependencies -----
# ----------------------------

# Debug and release config
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  # Turn on compiler warnings
  target_compile_options(quasar PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
  )
  # set debug defines
  target_compile_definitions(quasar PUBLIC
    QS_DEBUG
    QS_PROFILE
  )
endif()