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
        Quasar::Renderer2D::drawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
        Quasar::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        Quasar::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
        Quasar::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
        Quasar::Renderer2D::drawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_CheckerboardTexture, 20.0f);
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

    // taken from imgui_demo.cpp

    static bool dockspaceOpen = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            // ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            // ImGui::MenuItem("Padding", NULL, &opt_padding);
            // ImGui::Separator();

            if (ImGui::MenuItem("Exit")) { Quasar::Application::get().close(); }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Begin("Settings");

    auto stats = Quasar::Renderer2D::getStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.drawCalls);
    ImGui::Text("Quad Count: %d", stats.quadCount);
    ImGui::Text("Vertex Count: %d", stats.getTotalVertexCount());
    ImGui::Text("Index Count: %d", stats.getTotalIndexCount());

    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    
    uint32_t textureID = m_CheckerboardTexture->getRendererID();
    ImGui::Image((void *)textureID, ImVec2{ 256.0f, 256.0f });
    ImGui::End();

    ImGui::End();
}

void Sandbox2D::onEvent(Quasar::Event &e) 
{
    m_CameraController.onEvent(e);
}
