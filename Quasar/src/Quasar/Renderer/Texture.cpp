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
            return std::make_shared<OpenGLTexture2D>(path);
        
        default:
            QS_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }   
    }

} // namespace Quasar