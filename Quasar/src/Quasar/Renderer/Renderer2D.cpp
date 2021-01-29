#include "Renderer2D.hpp"

#include "Quasar/Core/Base.hpp"
#include "Quasar/Renderer/Shader.hpp"
#include "Quasar/Renderer/VertexArray.hpp"
#include "Quasar/Renderer/RenderCommand.hpp"

#include "Quasar/Debug/Instrumentor.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <array>

namespace Quasar
{

    struct QuadVertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoord;
        float texIndex;
        float tilingFactor;
    };

    struct Renderer2DData
    {
        static const uint32_t MAX_QUADS = 20000;
        static const uint32_t MAX_VERTICES = MAX_QUADS * 4;
        static const uint32_t MAX_INDICES = MAX_QUADS * 6;
        static const uint32_t MAX_TEXTURE_SLOTS = 32;

        Ref<VertexArray> quadVertexArray;
        Ref<VertexBuffer> quadVertexBuffer;
        Ref<Shader> textureShader;
        Ref<Texture2D> whiteTexture;

        uint32_t quadIndexCount = 0;
        QuadVertex *quadVertexBufferBase = nullptr;
        QuadVertex *quadVertexBufferPtr = nullptr;

        std::array<Ref<Texture2D>, MAX_TEXTURE_SLOTS> textureSlots;
        uint32_t textureSlotIndex = 1; // 0 = white texture

        glm::vec4 quadVertexPositions[4];

        Renderer2D::Statistics stats;
    };

    static Renderer2DData *s_Data;

    void Renderer2D::init() 
    {
        QS_PROFILE_FUNCTION();

        s_Data = new Renderer2DData();
        s_Data->quadVertexArray = VertexArray::create();

        s_Data->quadVertexBuffer = VertexBuffer::create(Renderer2DData::MAX_VERTICES * sizeof(QuadVertex));
        s_Data->quadVertexBuffer->setLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float, "a_TexIndex" },
            { ShaderDataType::Float, "a_TilingFactor" }
        });
        s_Data->quadVertexArray->addVertexBuffer(s_Data->quadVertexBuffer);

        s_Data->quadVertexBufferBase = new QuadVertex[Renderer2DData::MAX_VERTICES]; // TODO: free this memory

        uint32_t *quadIndices = new uint32_t[Renderer2DData::MAX_INDICES];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < Renderer2DData::MAX_INDICES; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        Ref<IndexBuffer> quadIB = IndexBuffer::create(quadIndices, Renderer2DData::MAX_INDICES);
        s_Data->quadVertexArray->setIndexBuffer(quadIB);
        delete[] quadIndices;

        s_Data->whiteTexture = Texture2D::create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data->whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

        int32_t samplers[Renderer2DData::MAX_TEXTURE_SLOTS];
        for (uint32_t i = 0; i < Renderer2DData::MAX_TEXTURE_SLOTS; i++)
        {
            samplers[i] = i;
        }
        
        s_Data->textureShader = Shader::create("/home/rvillegasm/dev/Quasar/sandbox/assets/shaders/Texture.glsl");
        s_Data->textureShader->bind();
        s_Data->textureShader->setIntArray("u_Textures", samplers, Renderer2DData::MAX_TEXTURE_SLOTS);

        s_Data->textureSlots[0] = s_Data->whiteTexture;

        s_Data->quadVertexPositions[0] = { -0.5f, -0.5, 0.0f, 1.0f };
        s_Data->quadVertexPositions[1] = {  0.5f, -0.5, 0.0f, 1.0f };
        s_Data->quadVertexPositions[2] = {  0.5f,  0.5, 0.0f, 1.0f };
        s_Data->quadVertexPositions[3] = { -0.5f,  0.5, 0.0f, 1.0f };
    }
    
    void Renderer2D::shutdown() 
    {
        QS_PROFILE_FUNCTION();

        delete[] s_Data->quadVertexBufferBase;
        delete s_Data;
    }
    
    void Renderer2D::beginScene(const Camera &camera, const glm::mat4 &transform) 
    {
        QS_PROFILE_FUNCTION();

        glm::mat4 viewProj = camera.getProjection() * glm::inverse(transform);

        s_Data->textureShader->bind();
        s_Data->textureShader->setMat4("u_ViewProjection", viewProj);

        s_Data->quadIndexCount = 0;
        s_Data->quadVertexBufferPtr = s_Data->quadVertexBufferBase;

        s_Data->textureSlotIndex = 1;
    }
    
    void Renderer2D::beginScene(const OrthographicCamera &camera) 
    {
        QS_PROFILE_FUNCTION();

        s_Data->textureShader->bind();
        s_Data->textureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());

        s_Data->quadIndexCount = 0;
        s_Data->quadVertexBufferPtr = s_Data->quadVertexBufferBase;

        s_Data->textureSlotIndex = 1;
    }
    
    void Renderer2D::endScene() 
    {
        QS_PROFILE_FUNCTION();

        uint32_t dataSize = (uint8_t *)s_Data->quadVertexBufferPtr - (uint8_t *)s_Data->quadVertexBufferBase;
        s_Data->quadVertexBuffer->setData(s_Data->quadVertexBufferBase, dataSize);

        flush();
    }

    void Renderer2D::flush()
    {
        QS_PROFILE_FUNCTION();

        if (s_Data->quadIndexCount == 0)
        {
            return; // There's nothing to draw
        }

        for (uint32_t i = 0; i < s_Data->textureSlotIndex; i++)
        {
            s_Data->textureSlots[i]->bind(i);
        }
        
        RenderCommand::drawIndexed(s_Data->quadVertexArray, s_Data->quadIndexCount);
        s_Data->stats.drawCalls++;
    }

    void Renderer2D::flushAndReset()
    {
        endScene();

        s_Data->quadIndexCount = 0;
        s_Data->quadVertexBufferPtr = s_Data->quadVertexBufferBase;

        s_Data->textureSlotIndex = 1;
    }
    
    void Renderer2D::drawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) 
    {
        drawQuad({ position.x, position.y, 0.0f }, size, color);
    }
    
    void Renderer2D::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) 
    {
        QS_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        
        drawQuad(transform, color);
    }
    
    void Renderer2D::drawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) 
    {
        drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
    }
    
    void Renderer2D::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) 
    {
        QS_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        
        drawQuad(transform, texture, tilingFactor, tintColor);
    }

    void Renderer2D::drawQuad(const glm::mat4 &transform, const glm::vec4 &color) 
    {
        QS_PROFILE_FUNCTION();

        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        const float textureIndex = 0.0f; // white texture
        const float tilingFactor = 1.0f;

        if (s_Data->quadIndexCount >= Renderer2DData::MAX_INDICES)
        {
            flushAndReset();
        }
        
        for (size_t i = 0; i < quadVertexCount; i++)
        {
            s_Data->quadVertexBufferPtr->position = transform * s_Data->quadVertexPositions[i];
            s_Data->quadVertexBufferPtr->color = color;
            s_Data->quadVertexBufferPtr->texCoord = textureCoords[i];
            s_Data->quadVertexBufferPtr->texIndex = textureIndex;
            s_Data->quadVertexBufferPtr->tilingFactor = tilingFactor;
            s_Data->quadVertexBufferPtr++;    
        }

        s_Data->quadIndexCount += 6;

        s_Data->stats.quadCount++;
    }

    void Renderer2D::drawQuad(const glm::mat4 &transform, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor)
    {
        QS_PROFILE_FUNCTION();

        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        if (s_Data->quadIndexCount >= Renderer2DData::MAX_INDICES)
        {
            flushAndReset();
        }

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data->textureSlotIndex; i++)
        {
            if (*s_Data->textureSlots[i] == *texture)
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            if (s_Data->textureSlotIndex >= Renderer2DData::MAX_TEXTURE_SLOTS)
            {
                flushAndReset();
            }

            textureIndex = (float)s_Data->textureSlotIndex;
            s_Data->textureSlots[s_Data->textureSlotIndex] = texture;
            s_Data->textureSlotIndex++;
        }
        
        for (size_t i = 0; i < quadVertexCount; i++)
        {
            s_Data->quadVertexBufferPtr->position = transform * s_Data->quadVertexPositions[i];
            s_Data->quadVertexBufferPtr->color = tintColor;
            s_Data->quadVertexBufferPtr->texCoord = textureCoords[i];
            s_Data->quadVertexBufferPtr->texIndex = textureIndex;
            s_Data->quadVertexBufferPtr->tilingFactor = tilingFactor;
            s_Data->quadVertexBufferPtr++;    
        }
        
        s_Data->quadIndexCount += 6;

        s_Data->stats.quadCount++;
    }

    void Renderer2D::drawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<SubTexture2D> &subtexture, float tilingFactor, const glm::vec4 &tintColor)
    {
        drawQuad({ position.x, position.y, 0.0f }, size, subtexture, tilingFactor, tintColor);
    }

    void Renderer2D::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<SubTexture2D> &subtexture, float tilingFactor, const glm::vec4 &tintColor)
    {
        QS_PROFILE_FUNCTION();

        constexpr size_t quadVertexCount = 4;
        const glm::vec2 *textureCoords = subtexture->getTexCoords();
        const Ref<Texture2D> texture = subtexture->getTexture();

        if (s_Data->quadIndexCount >= Renderer2DData::MAX_INDICES)
        {
            flushAndReset();
        }

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data->textureSlotIndex; i++)
        {
            if (*s_Data->textureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            if (s_Data->textureSlotIndex >= Renderer2DData::MAX_TEXTURE_SLOTS)
            {
                flushAndReset();
            }

            textureIndex = (float)s_Data->textureSlotIndex;
            s_Data->textureSlots[s_Data->textureSlotIndex] = texture;
            s_Data->textureSlotIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        
        for (size_t i = 0; i < quadVertexCount; i++)
        {
            s_Data->quadVertexBufferPtr->position = transform * s_Data->quadVertexPositions[i];
            s_Data->quadVertexBufferPtr->color = tintColor;
            s_Data->quadVertexBufferPtr->texCoord = textureCoords[i];
            s_Data->quadVertexBufferPtr->texIndex = textureIndex;
            s_Data->quadVertexBufferPtr->tilingFactor = tilingFactor;
            s_Data->quadVertexBufferPtr++;    
        }
        
        s_Data->quadIndexCount += 6;

        s_Data->stats.quadCount++;   
    }
    
    void Renderer2D::drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color) 
    {
        drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
    }
    
    void Renderer2D::drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color) 
    {
        QS_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        drawQuad(transform, color);
    }
    
    void Renderer2D::drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) 
    {
        drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
    }
    
    void Renderer2D::drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) 
    {
        QS_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        drawQuad(transform, texture, tilingFactor, tintColor);
    }

    void Renderer2D::drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const Ref<SubTexture2D> &subtexture, float tilingFactor, const glm::vec4 &tintColor) 
    {
        drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subtexture, tilingFactor, tintColor);
    }
    
    void Renderer2D::drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const Ref<SubTexture2D> &subtexture, float tilingFactor, const glm::vec4 &tintColor) 
    {
        QS_PROFILE_FUNCTION();

        constexpr size_t quadVertexCount = 4;
        const glm::vec2 *textureCoords = subtexture->getTexCoords();
        const Ref<Texture2D> texture = subtexture->getTexture();

        if (s_Data->quadIndexCount >= Renderer2DData::MAX_INDICES)
        {
            flushAndReset();
        }

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data->textureSlotIndex; i++)
        {
            if (*s_Data->textureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            if (s_Data->textureSlotIndex >= Renderer2DData::MAX_TEXTURE_SLOTS)
            {
                flushAndReset();
            }

            textureIndex = (float)s_Data->textureSlotIndex;
            s_Data->textureSlots[s_Data->textureSlotIndex] = texture;
            s_Data->textureSlotIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        for (size_t i = 0; i < quadVertexCount; i++)
        {
            s_Data->quadVertexBufferPtr->position = transform * s_Data->quadVertexPositions[i];
            s_Data->quadVertexBufferPtr->color = tintColor;
            s_Data->quadVertexBufferPtr->texCoord = textureCoords[i];
            s_Data->quadVertexBufferPtr->texIndex = textureIndex;
            s_Data->quadVertexBufferPtr->tilingFactor = tilingFactor;
            s_Data->quadVertexBufferPtr++;
        }
        
        s_Data->quadIndexCount += 6;

        s_Data->stats.quadCount++;
    }

    void Renderer2D::resetStats()
    {
        s_Data->stats.drawCalls = 0;
        s_Data->stats.quadCount = 0;
    }

    Renderer2D::Statistics Renderer2D::getStats()
    {
        return s_Data->stats;
    }

} // namespace Quasar
