#include "Sandbox2D.hpp"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::onAttach()
{
    QS_PROFILE_FUNCTION();

    m_CheckerboardTexture = Quasar::Texture2D::create("/home/rvillegasm/dev/Quasar/sandbox/assets/textures/Checkerboard.png");
}

void Sandbox2D::onDetach() 
{
    QS_PROFILE_FUNCTION();
    
}

void Sandbox2D::onUpdate(Quasar::Timestep ts) 
{
    QS_PROFILE_FUNCTION();

    // Update
    m_CameraController.onUpdate(ts);

    // Render
    {
        QS_PROFILE_SCOPE("Renderer Prep");
        Quasar::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Quasar::RenderCommand::clear();
    }

    {
        QS_PROFILE_SCOPE("Renderer Draw");
        Quasar::Renderer2D::beginScene(m_CameraController.getCamera());
        Quasar::Renderer2D::drawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
        Quasar::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
        Quasar::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 10.0f);
        Quasar::Renderer2D::endScene();
    }
}

void Sandbox2D::onImGuiRender() 
{
    QS_PROFILE_FUNCTION();

    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::onEvent(Quasar::Event &e) 
{
    m_CameraController.onEvent(e);
}
