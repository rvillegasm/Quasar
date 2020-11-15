#include "Texture.hpp"

#include "Quasar/Renderer/Renderer.hpp"

#include "Platform/OpenGL/OpenGLTexture.hpp"

namespace Quasar
{
    
    Ref<Texture2D> Texture2D::create(const std::string &path) 
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None:
            QS_CORE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;

        case RendererAPI::API::OpenGL:
            return createRef<OpenGLTexture2D>(path);
        
        default:
            QS_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }
    
    Ref<Texture2D> Texture2D::create(uint32_t width, uint32_t height) 
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None:
            QS_CORE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;

        case RendererAPI::API::OpenGL:
            return createRef<OpenGLTexture2D>(width, height);
        
        default:
            QS_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }

} // namespace Quasar