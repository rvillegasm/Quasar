#pragma once

#include <Quasar.hpp>

#include <vector> 

class Sandbox2D : public Quasar::Layer
{
private:
    Quasar::OrthographicCameraController m_CameraController;

    Quasar::Ref<Quasar::VertexArray> m_SquareVA;
    Quasar::Ref<Quasar::Shader> m_FlatColorShader;
    Quasar::Ref<Quasar::Framebuffer> m_Framebuffer;

    Quasar::Ref<Quasar::Texture2D> m_CheckerboardTexture;

    glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    void onAttach() override;
    void onDetach() override;

    void onUpdate(Quasar::Timestep ts) override;
    void onImGuiRender() override;
    void onEvent(Quasar::Event &e) override;
};