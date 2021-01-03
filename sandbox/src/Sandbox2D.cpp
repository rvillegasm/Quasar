#include "Sandbox2D.hpp"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
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
    Quasar::Renderer2D::resetStats();
    {
        QS_PROFILE_SCOPE("Renderer Prep");
        Quasar::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Quasar::RenderCommand::clear();
    }

    {
        static float rotation = 0.0f;
        rotation += ts * 50.0f;

        QS_PROFILE_SCOPE("Renderer Draw");
        Quasar::Renderer2D::beginScene(m_CameraController.getCamera());
        Quasar::Renderer2D::drawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
        Quasar::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        Quasar::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
        Quasar::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
        Quasar::Renderer2D::drawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, 20.0f);
        Quasar::Renderer2D::endScene();

        Quasar::Renderer2D::beginScene(m_CameraController.getCamera());
        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
                Quasar::Renderer2D::drawQuad({ x, y }, { 0.45f, 0.45f }, color);
            }
        }
        Quasar::Renderer2D::endScene();
    }
}

void Sandbox2D::onImGuiRender() 
{
    QS_PROFILE_FUNCTION();

    ImGui::Begin("Settings");

    auto stats = Quasar::Renderer2D::getStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.drawCalls);
    ImGui::Text("Quad Count: %d", stats.quadCount);
    ImGui::Text("Vertex Count: %d", stats.getTotalVertexCount());
    ImGui::Text("Index Count: %d", stats.getTotalIndexCount());

    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::onEvent(Quasar::Event &e) 
{
    m_CameraController.onEvent(e);
}
