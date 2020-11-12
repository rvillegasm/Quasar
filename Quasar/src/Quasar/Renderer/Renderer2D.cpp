#include "Renderer2D.hpp"

#include "Quasar/Core/Core.hpp"
#include "Quasar/Renderer/Shader.hpp"
#include "Quasar/Renderer/VertexArray.hpp"
#include "Quasar/Renderer/RenderCommand.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Quasar
{

    struct Renderer2DStorage
    {
        Ref<VertexArray> quadVertexArray;
        Ref<Shader> flatColorShader;
    };

    static Renderer2DStorage *s_Data;

    void Renderer2D::init() 
    {
        s_Data = new Renderer2DStorage();
        s_Data->quadVertexArray = VertexArray::create();

        float squareVertices[4 * 3] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
        };

        Ref<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::create(squareVertices, sizeof(squareVertices)));
        squareVB->setLayout({
            { ShaderDataType::Float3, "a_Position" }
        });
        s_Data->quadVertexArray->addVertexBuffer(squareVB);

        unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        Ref<Quasar::IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t))); 
        s_Data->quadVertexArray->setIndexBuffer(squareIB);

        s_Data->flatColorShader = Shader::create("/home/rvillegasm/dev/Quasar/sandbox/assets/shaders/FlatColorShader.glsl");
    }
    
    void Renderer2D::shutdown() 
    {
        delete s_Data;
    }
    
    void Renderer2D::beginScene(const OrthographicCamera &camera) 
    {
        s_Data->flatColorShader->bind();
        s_Data->flatColorShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
    }
    
    void Renderer2D::endScene() 
    {
        
    }
    
    void Renderer2D::drawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) 
    {
        drawQuad({ position.x, position.y, 0.0f }, size, color);
    }
    
    void Renderer2D::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) 
    {
        s_Data->flatColorShader->bind();
        s_Data->flatColorShader->setFloat4("u_Color", color);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_Data->flatColorShader->setMat4("u_Transform", transform);

        s_Data->quadVertexArray->bind();
        RenderCommand::drawIndexed(s_Data->quadVertexArray);
    }

} // namespace Quasar
