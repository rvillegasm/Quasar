#include "OpenGLUniformBuffer.hpp"

#include <glad/glad.h>

namespace Quasar
{

    OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
    {
        glCreateBuffers(1, &m_RendererID);
        glNamedBufferData(m_RendererID, 64, nullptr, GL_DYNAMIC_DRAW); // TODO: maybe use GL_STREAM_DRAW (?)
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_RendererID);
    }

    OpenGLUniformBuffer::~OpenGLUniformBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLUniformBuffer::setData(const void *data, uint32_t size, uint32_t offset)
    {
        glNamedBufferSubData(m_RendererID, offset, size, data);
    }

}