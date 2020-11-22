cmake_minimum_required(VERSION 3.16)

# project name
project(Quasar)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "ERROR: In-source builds are not allowed!. 
  You should create a separate directory for build files.")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(SOURCE_DIR "src")

# Source files
set(SOURCES 
  ${SOURCE_DIR}/Quasar/Core/Application.cpp
  ${SOURCE_DIR}/Quasar/Core/Log.cpp
  ${SOURCE_DIR}/Quasar/Core/Layer.cpp
  ${SOURCE_DIR}/Quasar/Core/LayerStack.cpp

  ${SOURCE_DIR}/Quasar/ImGui/ImGuiLayer.cpp
  ${SOURCE_DIR}/Quasar/ImGui/ImGuiBuild.cpp

  ${SOURCE_DIR}/Quasar/Renderer/GraphicsContext.cpp
  ${SOURCE_DIR}/Quasar/Renderer/Shader.cpp
  ${SOURCE_DIR}/Quasar/Renderer/Buffer.cpp
  ${SOURCE_DIR}/Quasar/Renderer/VertexArray.cpp
  ${SOURCE_DIR}/Quasar/Renderer/Renderer.cpp
  ${SOURCE_DIR}/Quasar/Renderer/Renderer2D.cpp
  ${SOURCE_DIR}/Quasar/Renderer/RenderCommand.cpp
  ${SOURCE_DIR}/Quasar/Renderer/RendererAPI.cpp
  ${SOURCE_DIR}/Quasar/Renderer/OrthographicCamera.cpp
  ${SOURCE_DIR}/Quasar/Renderer/OrthographicCameraController.cpp
  ${SOURCE_DIR}/Quasar/Renderer/Texture.cpp

  ${SOURCE_DIR}/Platform/OpenGL/OpenGLContext.cpp
  ${SOURCE_DIR}/Platform/OpenGL/OpenGLShader.cpp
  ${SOURCE_DIR}/Platform/OpenGL/OpenGLBuffer.cpp
  ${SOURCE_DIR}/Platform/OpenGL/OpenGLVertexArray.cpp
  ${SOURCE_DIR}/Platform/OpenGL/OpenGLRendererAPI.cpp
  ${SOURCE_DIR}/Platform/OpenGL/OpenGLTexture.cpp

  ${SOURCE_DIR}/Platform/Linux/LinuxWindow.cpp
  ${SOURCE_DIR}/Platform/Linux/LinuxInput.cpp

  vendor/stb_image/stb_image.cpp
)

# Add the library
add_library(quasar STATIC ${SOURCES})

# Set public API
set_target_properties(quasar PROPERTIES PUBLIC_HEADER src/Quasar.h)

# Top level include directories
target_include_directories(quasar PRIVATE src)

# ------------------------------------
# ----- Third party dependencies -----
# ------------------------------------

# spdlog
target_include_directories(quasar PUBLIC vendor/spdlog/include)
# GLFW (and OpenGL)
target_include_directories(quasar PUBLIC vendor/GLFW/include)
# glad
target_include_directories(quasar PUBLIC vendor/glad/include)
# ImGui
target_include_directories(quasar PUBLIC vendor/imgui)
# glm
target_include_directories(quasar PUBLIC vendor/glm)
# stb_image (compiled right here in this library)
target_include_directories(quasar PUBLIC vendor/stb_image)
# ------------------------------------
# --------------- END ----------------
# ------------------------------------

# compiler definitions
target_compile_definitions(quasar PUBLIC
  GLFW_INCLUDE_NONE # do not include opengl with glfw, since glad already does that
  # Debug flags (move to debug config later on)
  QS_ENABLE_ASSERTS
  QS_PROFILE
)
# show warnings on debug build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(quasar PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
      -Wall>
    $<$<CXX_COMPILER_ID:MSVC>:
      /W4>)
endif()
