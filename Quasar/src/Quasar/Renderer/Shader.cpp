#include "Shader.hpp"

#include "Quasar/Core/Base.hpp"
#include "Quasar/Renderer/Renderer.hpp"

#include "Platform/OpenGL/OpenGLShader.hpp"

namespace Quasar
{

    Ref<Shader> Shader::create(const std::string &filepath)
    {
        switch (Renderer::getAPI())
        {
            case RendererAPI::API::None:
                QS_CORE_ASSERT(false, "RendererAPI::None is not supported!");
                return nullptr;

            case RendererAPI::API::OpenGL:
                return createRef<OpenGLShader>(filepath);
            
            default:
                QS_CORE_ASSERT(false, "Unknown RendererAPI!");
                return nullptr;
        }
    }
    
    Ref<Shader> Shader::create(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc)
    {
        switch (Renderer::getAPI())
        {
            case RendererAPI::API::None:
                QS_CORE_ASSERT(false, "RendererAPI::None is not supported!");
                return nullptr;

            case RendererAPI::API::OpenGL:
                return createRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
            
            default:
                QS_CORE_ASSERT(false, "Unknown RendererAPI!");
                return nullptr;
        }
    }
    
    void ShaderLibrary::add(const Ref<Shader> &shader, const std::string &name)
    {
        QS_CORE_ASSERT(!exists(name), "Shader already exists!");
        m_Shaders[name] = shader;
    }

    void ShaderLibrary::add(const Ref<Shader> &shader) 
    {
        auto &name = shader->getName();
        add(shader, name);
    }
    
    Ref<Shader> ShaderLibrary::load(const std::string &filepath) 
    {
        auto shader = Shader::create(filepath);
        add(shader);
        return shader;
    }
    
    Ref<Shader> ShaderLibrary::load(const std::string &filepath, const std::string &name) 
    {
        auto shader = Shader::create(filepath);
        add(shader, name);
        return shader;
    }
    
    Ref<Shader> ShaderLibrary::get(const std::string &name) 
    {
        QS_CORE_ASSERT(exists(name), "Shader not found!");
        return m_Shaders[name];
    }
    
    bool ShaderLibrary::exists(const std::string &name) const
    {
        return m_Shaders.find(name) != m_Shaders.end();
    }

} // namespace Quasar
