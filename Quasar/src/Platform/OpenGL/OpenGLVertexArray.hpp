#pragma once

#include "Quasar/Renderer/VertexArray.hpp"

#include <vector>
#include <memory>
#include <cstdint>

namespace Quasar
{
    
    class OpenGLVertexArray final : public VertexArray
    {
    private:
        uint32_t m_RendererID;
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;

    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();

        void bind() const override;
        void unbind() const override;

        void addVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) override;
        void setIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) override;

        const std::vector<std::shared_ptr<VertexBuffer>> &getVertexBuffers() const override { return m_VertexBuffers; }
        const std::shared_ptr<IndexBuffer> &getIndexBuffer() const override { return m_IndexBuffer; }
    };

} // namespace Quasar
