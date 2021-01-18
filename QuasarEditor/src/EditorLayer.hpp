#pragma once

#include <Quasar.hpp>

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

        Ref<Texture2D> m_CheckerboardTexture;

        bool m_ViewportFocused = false;
        bool m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

        glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

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