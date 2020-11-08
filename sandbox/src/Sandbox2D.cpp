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
    m_SquareVA = Quasar::VertexArray::create();

    float squareVertices[4 * 3] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    Quasar::Ref<Quasar::VertexBuffer> squareVB;
    squareVB.reset(Quasar::VertexBuffer::create(squareVertices, sizeof(squareVertices)));
    squareVB->setLayout({
        { Quasar::ShaderDataType::Float3, "a_Position" }
    });
    m_SquareVA->addVertexBuffer(squareVB);

    unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
    Quasar::Ref<Quasar::IndexBuffer> squareIB;
    squareIB.reset(Quasar::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t))); 
    m_SquareVA->setIndexBuffer(squareIB);

    m_FlatColorShader = Quasar::Shader::create("/home/rvillegasm/dev/Quasar/sandbox/assets/shaders/FlatColorShader.glsl");
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

    Quasar::Renderer::beginScene(m_CameraController.getCamera());

    std::dynamic_pointer_cast<Quasar::OpenGLShader>(m_FlatColorShader)->bind();
    std::dynamic_pointer_cast<Quasar::OpenGLShader>(m_FlatColorShader)->uploadUniformFloat4("u_Color", m_SquareColor);

    Quasar::Renderer::submit(
        m_FlatColorShader,
        m_SquareVA,
        glm::scale(glm::mat4(1.0f), glm::vec3(1.5f))
    );

    Quasar::Renderer::endScene();
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
