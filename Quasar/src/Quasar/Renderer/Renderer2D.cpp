#include "Renderer2D.hpp"

#include "Quasar/Core/Core.hpp"
#include "Quasar/Renderer/Shader.hpp"
#include "Quasar/Renderer/VertexArray.hpp"
#include "Quasar/Renderer/RenderCommand.hpp"

#include "Quasar/Debug/Instrumentor.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Quasar
{

    struct QuadVertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoord;
    };

    struct Renderer2DData
    {
        const uint32_t MAX_QUADS = 10000;
        const uint32_t MAX_VERTICES = MAX_QUADS * 4;
        const uint32_t MAX_INDICES = MAX_VERTICES * 6;

        Ref<VertexArray> quadVertexArray;
        Ref<VertexBuffer> quadVertexBuffer;
        Ref<Shader> textureShader;
        Ref<Texture2D> whiteTexture;

        uint32_t quadIndexCount = 0;
        QuadVertex *quadVertexBufferBase = nullptr;
        QuadVertex *quadVertexBufferPtr = nullptr;
    };

    static Renderer2DData s_Data;

    void Renderer2D::init() 
    {
        QS_PROFILE_FUNCTION();

        s_Data.quadVertexArray = VertexArray::create();

        s_Data.quadVertexBuffer = VertexBuffer::create(s_Data.MAX_VERTICES * sizeof(QuadVertex));
        s_Data.quadVertexBuffer->setLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" }
        });
        s_Data.quadVertexArray->addVertexBuffer(s_Data.quadVertexBuffer);

        s_Data.quadVertexBufferBase = new QuadVertex[s_Data.MAX_VERTICES]; // TODO: free this memory

        uint32_t *quadIndices = new uint32_t[s_Data.MAX_INDICES];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MAX_INDICES; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        Ref<IndexBuffer> quadIB = IndexBuffer::create(quadIndices, s_Data.MAX_INDICES);
        s_Data.quadVertexArray->setIndexBuffer(quadIB);
        delete[] quadIndices;

        s_Data.whiteTexture = Texture2D::create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

        s_Data.textureShader = Shader::create("/home/rvillegasm/dev/Quasar/sandbox/assets/shaders/Texture.glsl");
        s_Data.textureShader->bind();
        s_Data.textureShader->setInt("u_Texture", 0);
    }
    
    void Renderer2D::shutdown() 
    {
        QS_PROFILE_FUNCTION();
    }
    
    void Renderer2D::beginScene(const OrthographicCamera &camera) 
    {
        QS_PROFILE_FUNCTION();

        s_Data.textureShader->bind();
        s_Data.textureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());

        s_Data.quadIndexCount = 0;
        s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;
    }
    
    void Renderer2D::endScene() 
    {
        QS_PROFILE_FUNCTION();

        uint32_t dataSize = (uint8_t *)s_Data.quadVertexBufferPtr - (uint8_t *)s_Data.quadVertexBufferBase;
        s_Data.quadVertexBuffer->setData(s_Data.quadVertexBufferBase, dataSize);

        flush();
    }

    void Renderer2D::flush()
    {
        QS_PROFILE_FUNCTION();

        RenderCommand::drawIndexed(s_Data.quadVertexArray, s_Data.quadIndexCount);
    }
    
    void Renderer2D::drawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) 
    {
        drawQuad({ position.x, position.y, 0.0f }, size, color);
    }
    
    void Renderer2D::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) 
    {
        QS_PROFILE_FUNCTION();

        s_Data.quadVertexBufferPtr->position = position;
        s_Data.quadVertexBufferPtr->color = color;
        s_Data.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
        s_Data.quadVertexBufferPtr++;

        s_Data.quadVertexBufferPtr->position = { position.x + size.x, position.y, 0.0f };
        s_Data.quadVertexBufferPtr->color = color;
        s_Data.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
        s_Data.quadVertexBufferPtr++;

        s_Data.quadVertexBufferPtr->position = { position.x + size.x, position.y + size.y, 0.0f };
        s_Data.quadVertexBufferPtr->color = color;
        s_Data.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
        s_Data.quadVertexBufferPtr++;

        s_Data.quadVertexBufferPtr->position = { position.x, position.y + size.y, 0.0f };
        s_Data.quadVertexBufferPtr->color = color;
        s_Data.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
        s_Data.quadVertexBufferPtr++;

        s_Data.quadIndexCount += 6;

        /* s_Data.textureShader->setFloat("u_TilingFactor", 1.0f);
        s_Data.whiteTexture->bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_Data.textureShader->setMat4("u_Transform", transform);
        s_Data.quadVertexArray->bind();
        RenderCommand::drawIndexed(s_Data.quadVertexArray); */
    }
    
    void Renderer2D::drawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) 
    {
        drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
    }
    
    void Renderer2D::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) 
    {
        QS_PROFILE_FUNCTION();

        s_Data.textureShader->setFloat4("u_Color", tintColor);
        s_Data.textureShader->setFloat("u_TilingFactor", tilingFactor);
        texture->bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_Data.textureShader->setMat4("u_Transform", transform);

        s_Data.quadVertexArray->bind();
        RenderCommand::drawIndexed(s_Data.quadVertexArray);
    }
    
    void Renderer2D::drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color) 
    {
        drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
    }
    
    void Renderer2D::drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color) 
    {
        QS_PROFILE_FUNCTION();

        s_Data.textureShader->setFloat4("u_Color", color);
        s_Data.textureShader->setFloat("u_TilingFactor", 1.0f);
        s_Data.whiteTexture->bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
            * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_Data.textureShader->setMat4("u_Transform", transform);

        s_Data.quadVertexArray->bind();
        RenderCommand::drawIndexed(s_Data.quadVertexArray);
    }
    
    void Renderer2D::drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) 
    {
        drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
    }
    
    void Renderer2D::drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) 
    {
        QS_PROFILE_FUNCTION();

        s_Data.textureShader->setFloat4("u_Color", tintColor);
        s_Data.textureShader->setFloat("u_TilingFactor", tilingFactor);
        texture->bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
            * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_Data.textureShader->setMat4("u_Transform", transform);

        s_Data.quadVertexArray->bind();
        RenderCommand::drawIndexed(s_Data.quadVertexArray);
    }

} // namespace Quasar
