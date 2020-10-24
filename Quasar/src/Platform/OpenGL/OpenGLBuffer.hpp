#pragma once

#include "Quasar/Renderer/Buffer.hpp"

namespace Quasar
{
    
    class OpenGLVertexBuffer : public VertexBuffer
    {
    private:
        uint32_t m_RendererID;

    public:
        OpenGLVertexBuffer(float *vertices, uint32_t size);
        ~OpenGLVertexBuffer();

        void bind() const override;
        void unbind() const override;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    private:
        uint32_t m_RendererID;
        uint32_t m_Count;

    public:
        OpenGLIndexBuffer(uint32_t *indices, uint32_t count);
        ~OpenGLIndexBuffer();

        void bind() const override;
        void unbind() const override;

        uint32_t getCount() const { return m_Count; };
    };

} // namespace Quasar

