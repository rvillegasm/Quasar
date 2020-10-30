#pragma once

#include "Quasar/Core.hpp"
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
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer> m_IndexBuffer;

    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();

        void bind() const override;
        void unbind() const override;

        void addVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) override;
        void setIndexBuffer(const Ref<IndexBuffer> &indexBuffer) override;

        const std::vector<Ref<VertexBuffer>> &getVertexBuffers() const override { return m_VertexBuffers; }
        const Ref<IndexBuffer> &getIndexBuffer() const override { return m_IndexBuffer; }
    };

} // namespace Quasar
