#pragma once

#include "Quasar/Renderer/Buffer.hpp"

namespace Quasar
{
    
    class OpenGLVertexBuffer final : public VertexBuffer
    {
    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;

    public:
        OpenGLVertexBuffer(uint32_t size);
        OpenGLVertexBuffer(float *vertices, uint32_t size);
        ~OpenGLVertexBuffer();

        void bind() const override;
        void unbind() const override;

        void setData(const void *data, uint32_t size) override;

        const BufferLayout &getLayout() const override { return m_Layout; }
        void setLayout(const BufferLayout &layout) override { m_Layout = layout; }
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

