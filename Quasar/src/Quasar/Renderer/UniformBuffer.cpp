#include "UniformBuffer.hpp"

#include "Quasar/Renderer/Renderer.hpp"

#include "Platform/OpenGL/OpenGLUniformBuffer.hpp"

namespace Quasar
{

    Ref<UniformBuffer> UniformBuffer::create(uint32_t size, uint32_t binding)
    {
        switch (Renderer::getAPI())
        {
            case RendererAPI::API::None:
                QS_CORE_ASSERT(false, "RendererAPI::None is not supported!");
                return nullptr;

            case RendererAPI::API::OpenGL:
                return createRef<OpenGLUniformBuffer>(size, binding);

            default:
                QS_CORE_ASSERT(false, "Unknown RendererAPI!");
                return nullptr;
        }
    }

}