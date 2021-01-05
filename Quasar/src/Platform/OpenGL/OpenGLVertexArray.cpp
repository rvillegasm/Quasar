#include "OpenGLVertexArray.hpp"

#include "Quasar/Core/Core.hpp"

#include "Quasar/Debug/Instrumentor.hpp"

#include <glad/glad.h>

namespace Quasar
{

    static GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:  return GL_FLOAT;
            case ShaderDataType::Float2: return GL_FLOAT;
            case ShaderDataType::Float3: return GL_FLOAT;
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Mat3:   return GL_FLOAT;
            case ShaderDataType::Mat4:   return GL_FLOAT;
            case ShaderDataType::Int:    return GL_INT;
            case ShaderDataType::Int2:   return GL_INT;
            case ShaderDataType::Int3:   return GL_INT;
            case ShaderDataType::Int4:   return GL_INT;
            case ShaderDataType::Bool:   return GL_BOOL;
        
            default:
                QS_CORE_ASSERT(false, "Unkown ShaderDataType!");
                return 0;
        }
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        QS_PROFILE_FUNCTION();

        glCreateVertexArrays(1, &m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        QS_PROFILE_FUNCTION();
        
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::bind() const
    {
        QS_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::unbind() const
    {
        QS_PROFILE_FUNCTION();

        glBindVertexArray(0);
    }

    void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer> &vertexBuffer)
    {
        QS_PROFILE_FUNCTION();

        const auto &layout = vertexBuffer->getLayout();
        QS_CORE_ASSERT(layout.getElements().size(), "VertexBuffer has no layout!");

        glBindVertexArray(m_RendererID);
        vertexBuffer->bind();

        for (const auto &element : layout)
        {
            switch (element.type)
            {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Bool:
            {
                glEnableVertexAttribArray(m_VertexBuffersIndex);
                glVertexAttribPointer(
                    m_VertexBuffersIndex,
                    element.getComponentCount(),
                    shaderDataTypeToOpenGLBaseType(element.type),
                    element.normalized ? GL_TRUE : GL_FALSE,
                    layout.getStride(),
                    (const void *)element.offset
                );
                m_VertexBuffersIndex++;
                break;
            }    
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
            {
                uint8_t count = element.getComponentCount();
                for (uint8_t i = 0; i < count; i++)
                {
                    glEnableVertexAttribArray(m_VertexBuffersIndex);
                    glVertexAttribPointer(
                        m_VertexBuffersIndex,
                        count,
                        shaderDataTypeToOpenGLBaseType(element.type),
                        element.normalized ? GL_TRUE : GL_FALSE,
                        layout.getStride(),
                        (const void *)(sizeof(float) * i)
                    );
                    glVertexAttribDivisor(m_VertexBuffersIndex, 1);
                    m_VertexBuffersIndex++;
                }
                break;
            }
            default:
                QS_CORE_ASSERT(false, "Unkown shaderDataType!");
                break;
            }
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer> &indexBuffer)
    {
        QS_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
        indexBuffer->bind();

        m_IndexBuffer = indexBuffer;
    }

} // namespace Quasar
