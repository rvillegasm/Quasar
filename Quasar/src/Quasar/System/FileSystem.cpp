#include "FileSystem.hpp"

namespace Quasar
{

    namespace Utils
    {
        static std::string shaderCacheAPIToString(ShaderCacheAPI api)
        {
            switch (api)
            {
                case ShaderCacheAPI::None:
                    return "none";
                case ShaderCacheAPI::OpenGL:
                    return "opengl";
                case ShaderCacheAPI::Vulkan:
                    return "vulkan";
            }
        }
    }

    std::filesystem::path FileSystem::s_CurrentPath = std::filesystem::current_path();
    
    std::string FileSystem::getAbsolutePath(std::string_view filepath)
    {
        return (s_CurrentPath / filepath).string();
    }
    
    std::string FileSystem::getAssetPath(std::string_view assetPath)
    {
        return (s_CurrentPath / "assets" / assetPath).string();
    }

    std::filesystem::path FileSystem::getShaderCacheDirectory(ShaderCacheAPI api)
    {
        return s_CurrentPath / "assets/cache/shader" / Utils::shaderCacheAPIToString(api);
    }

    void FileSystem::createShaderCacheDirectoryIfNeeded(ShaderCacheAPI api)
    {
        std::filesystem::path cacheDir = getShaderCacheDirectory(api);
        if (!std::filesystem::exists(cacheDir))
        {
            std::filesystem::create_directories(cacheDir);
        }
    }

} // namespace Quasar
