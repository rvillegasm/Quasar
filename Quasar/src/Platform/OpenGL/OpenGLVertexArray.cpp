#include "OpenGLVertexArray.hpp"

#include "Quasar/Core/Core.hpp"

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
        glCreateVertexArrays(1, &m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer> &vertexBuffer)
    {
        const auto &layout = vertexBuffer->getLayout();
        QS_CORE_ASSERT(layout.getElements().size(), "VertexBuffer has no layout!");

        glBindVertexArray(m_RendererID);
        vertexBuffer->bind();

        for (const auto &element : layout)
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
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer> &indexBuffer)
    {
        glBindVertexArray(m_RendererID);
        indexBuffer->bind();

        m_IndexBuffer = indexBuffer;
    }

} // namespace Quasar
