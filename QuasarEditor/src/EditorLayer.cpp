#include "EditorLayer.hpp"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Quasar
{
    
    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
    {
    }

    void EditorLayer::onAttach()
    {
        QS_PROFILE_FUNCTION();

        m_CheckerboardTexture = Texture2D::create("/home/rvillegasm/dev/Quasar/QuasarEditor/assets/textures/Checkerboard.png");

        FramebufferSpecification fbSpec;
        fbSpec.width = 1280;
        fbSpec.height = 720;
        m_Framebuffer = Framebuffer::create(fbSpec);

        m_ActiveScene = createRef<Scene>();

        auto square = m_ActiveScene->createEntity("Green Square");
        square.addComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

        auto redSquare = m_ActiveScene->createEntity("Red Square");
        redSquare.addComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

        m_SquareEntity = square;

        m_CameraEntity = m_ActiveScene->createEntity("Camera A");
        m_CameraEntity.addComponent<CameraComponent>();

        m_SecondCameraEntity = m_ActiveScene->createEntity("Camera B");
        auto &cc = m_SecondCameraEntity.addComponent<CameraComponent>();
        cc.primary = false;

        class CameraController : public  ScriptableEntity
        {
        public:
            void onCreate() override
            {
                auto &translation = getComponent<TransformComponent>().translation;
                translation.x = rand() % 10 - 5.0f;
            }
            void onDestroy() override
            {
            }

            void onUpdate(Timestep ts) override
            {
                auto &translation = getComponent<TransformComponent>().translation;

                float speed = 5.0f;

                if (Input::isKeyPressed(Key::A))
                {
                    translation.x -= speed * ts;
                }
                if (Input::isKeyPressed(Key::D))
                {
                    translation.x += speed * ts;
                }
                if (Input::isKeyPressed(Key::W))
                {
                    translation.y += speed * ts;
                }
                if (Input::isKeyPressed(Key::S))
                {
                    translation.y -= speed * ts;
                }
            }
        };

        m_CameraEntity.addComponent<NativeScriptComponent>().bind<CameraController>();
        m_SecondCameraEntity.addComponent<NativeScriptComponent>().bind<CameraController>();

        m_SceneHierarchyPanel.setContext(m_ActiveScene);
    }

    void EditorLayer::onDetach() 
    {
        QS_PROFILE_FUNCTION();
        
    }

    void EditorLayer::onUpdate(Timestep ts) 
    {
        QS_PROFILE_FUNCTION();

        // Resize
        FramebufferSpecification spec = m_Framebuffer->getSpecification();
        if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f 
            && (spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y))
        {
            m_Framebuffer->resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.onResize(m_ViewportSize.x, m_ViewportSize.y);

            m_ActiveScene->onViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        // Update
        if (m_ViewportFocused)
        {
            m_CameraController.onUpdate(ts);
        }

        // Render
        Renderer2D::resetStats();
        m_Framebuffer->bind();
        RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        RenderCommand::clear();

        // Update scene
        m_ActiveScene->onUpdate(ts);        

        m_Framebuffer->unbind();
    }

    void EditorLayer::onImGuiRender() 
    {
        QS_PROFILE_FUNCTION();

        // taken from imgui_demo.cpp and modified
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

                if (ImGui::MenuItem("Exit")) { Application::get().close(); }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        m_SceneHierarchyPanel.onImGuiRender();

        ImGui::Begin("Stats");

        auto stats = Renderer2D::getStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quad Count: %d", stats.quadCount);
        ImGui::Text("Vertex Count: %d", stats.getTotalVertexCount());
        ImGui::Text("Index Count: %d", stats.getTotalIndexCount());
        
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::get().getImGuiLayer()->blockEvents(!m_ViewportFocused || !m_ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        uint64_t textureID = m_Framebuffer->getColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void *>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();

    }

    void EditorLayer::onEvent(Event &e) 
    {
        m_CameraController.onEvent(e);
    }

} // namespace Quasar

