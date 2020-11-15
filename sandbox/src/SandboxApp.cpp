#include <Quasar.hpp>
#include <Quasar/Core/EntryPoint.hpp>

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Sandbox2D.hpp"

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
    ExampleLayer()
        : Layer("Example"), m_CameraController(1280.0f / 720.0f)
    {
        m_VertexArray = Quasar::VertexArray::create();

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        Quasar::Ref<Quasar::VertexBuffer> vertexBuffer = Quasar::VertexBuffer::create(vertices, sizeof(vertices));
        Quasar::BufferLayout layout = {
            { Quasar::ShaderDataType::Float3, "a_Position" },
            { Quasar::ShaderDataType::Float4, "a_Color" },
        };
        vertexBuffer->setLayout(layout);
        m_VertexArray->addVertexBuffer(vertexBuffer);

        unsigned int indices[3] = { 0, 1, 2 };
        Quasar::Ref<Quasar::IndexBuffer> indexBuffer = Quasar::IndexBuffer::create(
            indices,
            sizeof(indices) / sizeof(uint32_t)
        );
        
        m_VertexArray->setIndexBuffer(indexBuffer);

        m_SquareVA = Quasar::VertexArray::create();
        float squareVertices[4 * 5] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        Quasar::Ref<Quasar::VertexBuffer> squareVB = Quasar::VertexBuffer::create(
            squareVertices,
            sizeof(squareVertices)
        );
        squareVB->setLayout({
            { Quasar::ShaderDataType::Float3, "a_Position" },
            { Quasar::ShaderDataType::Float2, "a_TexCoord" },
        });
        m_SquareVA->addVertexBuffer(squareVB);

        unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        Quasar::Ref<Quasar::IndexBuffer> squareIB = Quasar::IndexBuffer::create(
            squareIndices,
            sizeof(squareIndices) / sizeof(uint32_t)
        );
        m_SquareVA->setIndexBuffer(squareIB);

        const std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

        m_Shader = Quasar::Shader::create("VertexPosColor", vertexSrc, fragmentSrc);

        const std::string flatColorShaderVertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;

            void main()
            {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

        const std::string flatColorShaderFragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;

            uniform vec3 u_Color;

            void main()
            {
                color = vec4(u_Color, 1.0);
            }
        )";

        m_FlatColorShader = Quasar::Shader::create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

        auto textureShader = m_ShaderLibrary.load("/home/rvillegasm/dev/Quasar/sandbox/assets/shaders/Texture.glsl");

        m_Texture = Quasar::Texture2D::create("/home/rvillegasm/dev/Quasar/sandbox/assets/textures/Checkerboard.png");
        m_ChernoLogoTexture = Quasar::Texture2D::create("/home/rvillegasm/dev/Quasar/sandbox/assets/textures/ChernoLogo.png");

        textureShader->bind();
        textureShader->setInt("u_Texture", 0);
    }

    void onUpdate(Quasar::Timestep ts) override
    {
        // Update
        m_CameraController.onUpdate(ts);

        // Render
        Quasar::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Quasar::RenderCommand::clear();

        Quasar::Renderer::beginScene(m_CameraController.getCamera());

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        m_FlatColorShader->bind();
        m_FlatColorShader->setFloat3("u_Color", m_SquareColor);

        for (int y = 0; y < 20; y++)
        {
            for (int x = 0; x < 20; x++)
            {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

                Quasar::Renderer::submit(m_FlatColorShader, m_SquareVA, transform);
            }
        }

        auto textureShader = m_ShaderLibrary.get("Texture");

        m_Texture->bind();
        Quasar::Renderer::submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
        m_ChernoLogoTexture->bind();
        Quasar::Renderer::submit(
            textureShader,
            m_SquareVA,
            glm::scale(glm::mat4(1.0f), glm::vec3(1.5f))
        );

        // Triangle
        // Quasar::Renderer::submit(m_Shader, m_VertexArray);
        
        Quasar::Renderer::endScene();
    }

    void onImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
        ImGui::End();
    }

    void onEvent(Quasar::Event &e) override
    {
        m_CameraController.onEvent(e);
    }

};

class Sandbox : public Quasar::Application
{
public:
    Sandbox() 
    {
        // pushLayer(new ExampleLayer());
        pushLayer(new Sandbox2D());
    }

    ~Sandbox() 
    {
    }
};

Quasar::Application *Quasar::createApplication()
{
    return new Sandbox();
}