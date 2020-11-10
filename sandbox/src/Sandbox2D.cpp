#include "Sandbox2D.hpp"

#include "Platform/OpenGL/OpenGLShader.hpp"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::onAttach() 
{
    
}

void Sandbox2D::onDetach() 
{
    
}

void Sandbox2D::onUpdate(Quasar::Timestep ts) 
{
    // Update
    m_CameraController.onUpdate(ts);

    // Render
    Quasar::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Quasar::RenderCommand::clear();

    Quasar::Renderer2D::beginScene(m_CameraController.getCamera());

    Quasar::Renderer2D::drawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, {0.8f, 0.2f, 0.3f, 1.0f});

    Quasar::Renderer2D::endScene();
    // std::dynamic_pointer_cast<Quasar::OpenGLShader>(m_FlatColorShader)->bind();
    // std::dynamic_pointer_cast<Quasar::OpenGLShader>(m_FlatColorShader)->uploadUniformFloat4("u_Color", m_SquareColor);  

}

void Sandbox2D::onImGuiRender() 
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::onEvent(Quasar::Event &e) 
{
    m_CameraController.onEvent(e);
}
