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
  ${SOURCE_DIR}/Quasar/Core/Window.cpp

  ${SOURCE_DIR}/Quasar/System/FileSystem.cpp

  ${SOURCE_DIR}/Quasar/ImGui/ImGuiLayer.cpp
  ${SOURCE_DIR}/Quasar/ImGui/ImGuiBuild.cpp

  ${SOURCE_DIR}/Quasar/Renderer/GraphicsContext.cpp
  ${SOURCE_DIR}/Quasar/Renderer/Shader.cpp
  ${SOURCE_DIR}/Quasar/Renderer/Buffer.cpp
  ${SOURCE_DIR}/Quasar/Renderer/Framebuffer.cpp
  ${SOURCE_DIR}/Quasar/Renderer/VertexArray.cpp
  ${SOURCE_DIR}/Quasar/Renderer/Renderer.cpp
  ${SOURCE_DIR}/Quasar/Renderer/Renderer2D.cpp
  ${SOURCE_DIR}/Quasar/Renderer/RenderCommand.cpp
  ${SOURCE_DIR}/Quasar/Renderer/RendererAPI.cpp
  ${SOURCE_DIR}/Quasar/Renderer/OrthographicCamera.cpp
  ${SOURCE_DIR}/Quasar/Renderer/OrthographicCameraController.cpp
  ${SOURCE_DIR}/Quasar/Renderer/Texture.cpp
  ${SOURCE_DIR}/Quasar/Renderer/SubTexture2D.cpp

  ${SOURCE_DIR}/Platform/OpenGL/OpenGLContext.cpp
  ${SOURCE_DIR}/Platform/OpenGL/OpenGLShader.cpp
  ${SOURCE_DIR}/Platform/OpenGL/OpenGLBuffer.cpp
  ${SOURCE_DIR}/Platform/OpenGL/OpenGLFramebuffer.cpp
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
set(QUASAR_DEPENDENCIES_DIR "vendor")

# spdlog
target_include_directories(quasar PUBLIC ${QUASAR_DEPENDENCIES_DIR}/spdlog/include)

# GLFW (and OpenGL)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${QUASAR_DEPENDENCIES_DIR}/GLFW)
target_include_directories(quasar PUBLIC ${QUASAR_DEPENDENCIES_DIR}/GLFW/include)

# glad
add_subdirectory(${QUASAR_DEPENDENCIES_DIR}/glad)
target_include_directories(quasar PUBLIC ${QUASAR_DEPENDENCIES_DIR}/glad/include)

# ImGui
target_include_directories(quasar PUBLIC ${QUASAR_DEPENDENCIES_DIR}/imgui)
set(IMGUI_SOURCES
  ${QUASAR_DEPENDENCIES_DIR}/imgui/imgui.cpp
  ${QUASAR_DEPENDENCIES_DIR}/imgui/imgui_widgets.cpp
  ${QUASAR_DEPENDENCIES_DIR}/imgui/imgui_tables.cpp
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

# glm
target_include_directories(quasar PUBLIC ${QUASAR_DEPENDENCIES_DIR}/glm)

# stb_image (compiled right here in this library)
target_include_directories(quasar PUBLIC ${QUASAR_DEPENDENCIES_DIR}/stb_image)
# ------------------------------------
# --------------- END ----------------
# ------------------------------------

# compiler definitions
target_compile_definitions(quasar PUBLIC
  GLFW_INCLUDE_NONE # do not include opengl with glfw, since glad already does that
  # Debug flags (move to debug config later on)
  QS_DEBUG
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
