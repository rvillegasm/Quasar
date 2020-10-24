#include "Buffer.hpp"

#include "Quasar/Core.hpp"
#include "Quasar/Renderer/Renderer.hpp"

#include "Platform/OpenGL/OpenGLBuffer.hpp"

namespace Quasar
{

    VertexBuffer *VertexBuffer::create(float *vertices, uint32_t size)
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::None:
            QS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;

        case RendererAPI::OpenGL:
            return new OpenGLVertexBuffer(vertices, size);

        default:
            QS_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }

    IndexBuffer *IndexBuffer::create(uint32_t *indices, uint32_t count)
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::None:
            QS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;

        case RendererAPI::OpenGL:
            return new OpenGLIndexBuffer(indices, count);

        default:
            QS_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }

} // namespace Quasar
