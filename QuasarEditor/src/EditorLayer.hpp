#pragma once

#include <Quasar.hpp>
#include <Quasar/Renderer/EditorCamera.hpp>
#include "Panels/SceneHierarchyPanel.hpp"
#include "Panels/ContentBrowserPanel.hpp"

#include <glm/glm.hpp>

#include <vector>

namespace Quasar
{
    
    class EditorLayer : public Layer
    {
    private:
        OrthographicCameraController m_CameraController;

        Ref<VertexArray> m_SquareVA;
        Ref<Shader> m_FlatColorShader;
        Ref<Framebuffer> m_Framebuffer;

        Ref<Scene> m_ActiveScene;
        Entity m_SquareEntity;
        Entity m_CameraEntity;
        Entity m_SecondCameraEntity;

        Entity m_HoveredEntity;
        
        bool m_PrimaryCamera = true;

        EditorCamera m_EditorCamera;

        Ref<Texture2D> m_CheckerboardTexture;

        bool m_ViewportFocused = false;
        bool m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        glm::vec2 m_ViewportBounds[2];

        glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

        int m_GizmoType = -1;

        // Panels
        SceneHierarchyPanel m_SceneHierarchyPanel;
        ContentBrowserPanel m_ContentBrowserPanel;

        bool onKeyPressed(KeyPressedEvent &e);
        bool onMouseButtonPressed(MouseButtonPressedEvent &e);

        void startNewScene();
        void openScene();
        void saveSceneAs();

    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        void onAttach() override;
        void onDetach() override;

        void onUpdate(Timestep ts) override;
        void onImGuiRender() override;
        void onEvent(Event &e) override;
    };

} // namespace Quasar
