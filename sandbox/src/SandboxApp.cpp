#include <Quasar.hpp>

#include "imgui.h"
#include <glm/glm.hpp>

class ExampleLayer : public Quasar::Layer
{
private:
    std::shared_ptr<Quasar::Shader> m_Shader;
    std::shared_ptr<Quasar::VertexArray> m_VertexArray;

    std::shared_ptr<Quasar::Shader> m_BlueShader;
    std::shared_ptr<Quasar::VertexArray> m_SquareVA;

    Quasar::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraMoveSpeed = 5.0f;

    float m_CameraRotation = 0.0f;
    float m_CameraRotationSpeed = 180.0f;

public:
    ExampleLayer()
        : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
    {
        m_VertexArray.reset(Quasar::VertexArray::create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        std::shared_ptr<Quasar::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Quasar::VertexBuffer::create(vertices, sizeof(vertices)));
        Quasar::BufferLayout layout = {
            { Quasar::ShaderDataType::Float3, "a_Position" },
            { Quasar::ShaderDataType::Float4, "a_Color" },
        };
        vertexBuffer->setLayout(layout);
        m_VertexArray->addVertexBuffer(vertexBuffer);

        unsigned int indices[3] = { 0, 1, 2 };
        std::shared_ptr<Quasar::IndexBuffer> indexBuffer;
        indexBuffer.reset(Quasar::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->setIndexBuffer(indexBuffer);

        m_SquareVA.reset(Quasar::VertexArray::create());
        float squareVertices[4 * 3] = {
            -0.75f, -0.75f, 0.0f,
             0.75f, -0.75f, 0.0f,
             0.75f,  0.75f, 0.0f,
            -0.75f,  0.75f, 0.0f
        };

        std::shared_ptr<Quasar::VertexBuffer> squareVB;
        squareVB.reset(Quasar::VertexBuffer::create(squareVertices, sizeof(squareVertices)));
        squareVB->setLayout({
            { Quasar::ShaderDataType::Float3, "a_Position" },
        });
        m_SquareVA->addVertexBuffer(squareVB);

        unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<Quasar::IndexBuffer> squareIB;
        squareIB.reset(Quasar::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t))); 
        m_SquareVA->setIndexBuffer(squareIB);

        const std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";

        const std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
            }
        )";

        m_Shader = std::make_shared<Quasar::Shader>(vertexSrc, fragmentSrc);

        const std::string blueShaderVertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;

            out vec3 v_Position;

            void main()
            {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";

        const std::string blueShaderFragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;

            void main()
            {
                color = vec4(0.2, 0.3, 0.8, 1.0);
            }
        )";

        m_BlueShader = std::make_shared<Quasar::Shader>(blueShaderVertexSrc, blueShaderFragmentSrc);
    }

    void onUpdate(Quasar::Timestep ts) override
    {
        QS_TRACE("Delta time: {0}s ({1}ms)", ts.getSeconds(), ts.getMilliseconds());

        if (Quasar::Input::isKeyPressed(QS_KEY_LEFT))
        {
            m_CameraPosition.x -= m_CameraMoveSpeed * ts;
        }
        else if (Quasar::Input::isKeyPressed(QS_KEY_RIGHT))
        {
            m_CameraPosition.x += m_CameraMoveSpeed * ts;
        }

        if (Quasar::Input::isKeyPressed(QS_KEY_UP))
        {
            m_CameraPosition.y += m_CameraMoveSpeed * ts;
        }
        else if (Quasar::Input::isKeyPressed(QS_KEY_DOWN))
        {
            m_CameraPosition.y -= m_CameraMoveSpeed * ts;
        }

        if (Quasar::Input::isKeyPressed(QS_KEY_A))
        {
            m_CameraRotation += m_CameraRotationSpeed * ts;
        }
        else if (Quasar::Input::isKeyPressed(QS_KEY_D))
        {
            m_CameraRotation -= m_CameraRotationSpeed * ts;
        }


        Quasar::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Quasar::RenderCommand::clear();

        m_Camera.setPosition(m_CameraPosition);
        m_Camera.setRotation(m_CameraRotation);

        Quasar::Renderer::beginScene(m_Camera);
        
        Quasar::Renderer::submit(m_BlueShader, m_SquareVA);
        Quasar::Renderer::submit(m_Shader, m_VertexArray);
        
        Quasar::Renderer::endScene();   
    }

    void onImGuiRender() override
    {
    }

    void onEvent(Quasar::Event &event) override
    {
    }

};

class Sandbox : public Quasar::Application
{
public:
    Sandbox() 
    {
        pushLayer(new ExampleLayer());
    }

    ~Sandbox() 
    {
    }
};

Quasar::Application *Quasar::createApplication()
{
    return new Sandbox();
}