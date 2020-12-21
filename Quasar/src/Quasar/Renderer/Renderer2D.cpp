#include "Renderer2D.hpp"

#include "Quasar/Core/Core.hpp"
#include "Quasar/Renderer/Shader.hpp"
#include "Quasar/Renderer/VertexArray.hpp"
#include "Quasar/Renderer/RenderCommand.hpp"

#include "Quasar/Debug/Instrumentor.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Quasar
{

    struct Renderer2DStorage
    {
        Ref<VertexArray> quadVertexArray;
        Ref<Shader> textureShader;
        Ref<Texture2D> whiteTexture;
    };

    static Renderer2DStorage *s_Data;

    void Renderer2D::init() 
    {
        QS_PROFILE_FUNCTION();

        s_Data = new Renderer2DStorage();
        s_Data->quadVertexArray = VertexArray::create();

        float squareVertices[4 * 5] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        Ref<VertexBuffer> squareVB = VertexBuffer::create(squareVertices, sizeof(squareVertices));
        
        squareVB->setLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" }
        });
        s_Data->quadVertexArray->addVertexBuffer(squareVB);

        unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        
        Ref<IndexBuffer> squareIB = IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

        s_Data->quadVertexArray->setIndexBuffer(squareIB);

        s_Data->whiteTexture = Texture2D::create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data->whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

        s_Data->textureShader = Shader::create("/home/rvillegasm/dev/Quasar/sandbox/assets/shaders/Texture.glsl");
        s_Data->textureShader->bind();
        s_Data->textureShader->setInt("u_Texture", 0);
    }
    
    void Renderer2D::shutdown() 
    {
        QS_PROFILE_FUNCTION();

        delete s_Data;
    }
    
    void Renderer2D::beginScene(const OrthographicCamera &camera) 
    {
        QS_PROFILE_FUNCTION();

        s_Data->textureShader->bind();
        s_Data->textureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
    }
    
    void Renderer2D::endScene() 
    {
        QS_PROFILE_FUNCTION();

    }
    
    void Renderer2D::drawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) 
    {
        drawQuad({ position.x, position.y, 0.0f }, size, color);
    }
    
    void Renderer2D::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) 
    {
        QS_PROFILE_FUNCTION();

        s_Data->textureShader->setFloat4("u_Color", color);
        s_Data->textureShader->setFloat("u_TilingFactor", 1.0f);
        s_Data->whiteTexture->bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_Data->textureShader->setMat4("u_Transform", transform);

        s_Data->quadVertexArray->bind();
        RenderCommand::drawIndexed(s_Data->quadVertexArray);
    }
    
    void Renderer2D::drawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) 
    {
        drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
    }
    
    void Renderer2D::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) 
    {
        QS_PROFILE_FUNCTION();

        s_Data->textureShader->setFloat4("u_Color", tintColor);
        s_Data->textureShader->setFloat("u_TilingFactor", tilingFactor);
        texture->bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_Data->textureShader->setMat4("u_Transform", transform);

        s_Data->quadVertexArray->bind();
        RenderCommand::drawIndexed(s_Data->quadVertexArray);
    }
    
    void Renderer2D::drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color) 
    {
        drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
    }
    
    void Renderer2D::drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color) 
    {
        QS_PROFILE_FUNCTION();

        s_Data->textureShader->setFloat4("u_Color", color);
        s_Data->textureShader->setFloat("u_TilingFactor", 1.0f);
        s_Data->whiteTexture->bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
            * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_Data->textureShader->setMat4("u_Transform", transform);

        s_Data->quadVertexArray->bind();
        RenderCommand::drawIndexed(s_Data->quadVertexArray);
    }
    
    void Renderer2D::drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) 
    {
        drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
    }
    
    void Renderer2D::drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) 
    {
        QS_PROFILE_FUNCTION();

        s_Data->textureShader->setFloat4("u_Color", tintColor);
        s_Data->textureShader->setFloat("u_TilingFactor", tilingFactor);
        texture->bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
            * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_Data->textureShader->setMat4("u_Transform", transform);

        s_Data->quadVertexArray->bind();
        RenderCommand::drawIndexed(s_Data->quadVertexArray);
    }

} // namespace Quasar
