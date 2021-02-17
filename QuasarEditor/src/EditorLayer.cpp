#include "EditorLayer.hpp"

#include <Quasar/Scene/SceneSerializer.hpp>
#include <Quasar/Math/Math.hpp>
#include <Quasar/System/FileSystem.hpp>
#include <Quasar/Utils/PlatformUtils.hpp>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ImGuizmo.h>

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
        fbSpec.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
        fbSpec.width = 1280;
        fbSpec.height = 720;
        m_Framebuffer = Framebuffer::create(fbSpec);

        m_ActiveScene = createRef<Scene>();

        m_EditorCamera = EditorCamera(glm::radians(30.0f), 1.778f, 0.1f, 1000.f);
#if 0
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
#endif
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
            m_EditorCamera.setViewportSize(m_ViewportSize.x, m_ViewportSize.y);

            m_ActiveScene->onViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        // Update
        if (m_ViewportFocused)
        {
            m_CameraController.onUpdate(ts);
        }

        m_EditorCamera.onUpdate(ts);

        // Render
        Renderer2D::resetStats();
        m_Framebuffer->bind();
        RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        RenderCommand::clear();

        // Update scene
        m_ActiveScene->onUpdateEditor(ts, m_EditorCamera);

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
        ImGuiStyle &style = ImGui::GetStyle();
        float defaultMinWindowSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = defaultMinWindowSizeX;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                // ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                // ImGui::MenuItem("Padding", NULL, &opt_padding);
                // ImGui::Separator();

                if (ImGui::MenuItem("New", "Ctrl+N"))
                {
                    startNewScene();
                }

                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                {
                    openScene();
                }

                if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S"))
                {
                    saveSceneAs();
                }

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
        Application::get().getImGuiLayer()->blockEvents(!m_ViewportFocused && !m_ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        uint64_t textureID = m_Framebuffer->getColorAttachmentRendererID(0);
        ImGui::Image(reinterpret_cast<void *>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        // Gizmos
        Entity selectedEntity = m_SceneHierarchyPanel.getSelectedEntity();
        if (selectedEntity && m_GizmoType != -1)
        {
            ImGuizmo::SetOrthographic(false); // TODO: make orthigraphic guizmos work
            ImGuizmo::SetDrawlist();

            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            // camera

            // Runtime Camera from entity
            // auto cameraEntity = m_ActiveScene->getPrimaryCameraEntity();
            // const auto &camera = cameraEntity.getComponent<CameraComponent>().camera;

            // const glm::mat4 &cameraProjection = camera.getProjection();
            // glm::mat4 cameraView = glm::inverse(cameraEntity.getComponent<TransformComponent>().getTransform());

            // Editor Camera
            const glm::mat4 &cameraProjection = m_EditorCamera.getProjection();
            glm::mat4 cameraView = m_EditorCamera.getViewMatrix();

            // Entity transform
            auto &transformComponent = selectedEntity.getComponent<TransformComponent>();
            glm::mat4 transform = transformComponent.getTransform();

            // Snapping
            bool snap = Input::isKeyPressed(Key::LeftControl);
            float snapValue = 0.5f; // snap to 0.5m for T and S
            if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
            {
                snapValue = 45.0f; // snap to 45 degrees for R
            }

            float snapValues[3] = { snapValue, snapValue, snapValue };

            ImGuizmo::Manipulate(
                glm::value_ptr(cameraView),
                glm::value_ptr(cameraProjection),
                static_cast<ImGuizmo::OPERATION>(m_GizmoType),
                ImGuizmo::LOCAL,
                glm::value_ptr(transform),
                nullptr,
                snap ? snapValues : nullptr
            );

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 translation, rotation, scale;
                Math::decomposeTransform(transform, translation, rotation, scale);

                glm::vec3 deltaRotation = rotation - transformComponent.rotation;
                transformComponent.translation = translation;
                transformComponent.rotation += deltaRotation; // prevent gimbal lock (according to cherno) TODO: Look into that
                transformComponent.scale = scale;
            }
        }
        
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();

    }

    void EditorLayer::onEvent(Event &e) 
    {
        m_CameraController.onEvent(e);
        m_EditorCamera.onEvent(e);

        EventDispatcher dispatcher(e);
        dispatcher.dispatch<KeyPressedEvent>(QS_BIND_EVENT_FN(EditorLayer::onKeyPressed));
    }

    bool EditorLayer::onKeyPressed(KeyPressedEvent &e) 
    {
        // Editor-level Shortcuts
        if (e.getRepeatCount() > 0)
        {
            return false;
        }

        bool isControlKeyPressed = Input::isKeyPressed(Key::LeftControl) || Input::isKeyPressed(Key::RightControl);
        bool isShiftKeyPressed = Input::isKeyPressed(Key::LeftShift) || Input::isKeyPressed(Key::RightShift);
        switch (e.getKeyCode())
        {
        case Key::N:
            if (isControlKeyPressed)
            {
                startNewScene();
                return true;
            }
            break;

        case Key::O:
            if (isControlKeyPressed)
            {
                openScene();
                return true;
            }
            break;

        case Key::S:
            if (isControlKeyPressed && isShiftKeyPressed)
            {
                saveSceneAs();
                return true;
            }
            break;
        
        // Gizmos
        case Key::Q:
            m_GizmoType = -1;
            break;

        case Key::W:
            m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            break;

        case Key::E:
            m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            break;

        case Key::R:
            m_GizmoType = ImGuizmo::OPERATION::SCALE;
            break;

        default:
            break;
        }
        return false;
    }

    void EditorLayer::startNewScene() 
    {
        m_ActiveScene = createRef<Scene>();
        m_ActiveScene->onViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.setContext(m_ActiveScene);
    }

    void EditorLayer::openScene() 
    {
        std::optional<std::string> filepath = FileDialogs::openFile({ "Quasar Scenes (*.qscene)", "*.qscene" });
        if (filepath)
        {
            startNewScene();
            SceneSerializer serializer(m_ActiveScene);
            serializer.deserializeFromText(filepath.value());
        }
    }

    void EditorLayer::saveSceneAs() 
    {
        std::optional<std::string> filepath = FileDialogs::saveFile({ "Quasar Scenes (*.qscene)", "*.qscene" });
        if (filepath)
        {
            SceneSerializer serializer(m_ActiveScene);
            serializer.serializeToText(filepath.value());
        }
    }

} // namespace Quasar

