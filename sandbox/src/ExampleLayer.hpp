#pragma once

#include <Quasar.hpp>
#include <glm/glm.hpp>

class ExampleLayer : public Quasar::Layer
{
private:
    Quasar::ShaderLibrary m_ShaderLibrary;
    Quasar::Ref<Quasar::Shader> m_Shader;
    Quasar::Ref<Quasar::VertexArray> m_VertexArray;

    Quasar::Ref<Quasar::Shader> m_FlatColorShader;
    Quasar::Ref<Quasar::VertexArray> m_SquareVA;

    Quasar::Ref<Quasar::Texture2D> m_Texture, m_ChernoLogoTexture;

    Quasar::OrthographicCameraController m_CameraController;
    glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };

public:
    ExampleLayer();
    virtual ~ExampleLayer() = default;

    void onAttach() override;
    void onDetach() override;

    void onUpdate(Quasar::Timestep ts) override;
    void onImGuiRender() override;
    void onEvent(Quasar::Event &e) override;
};