#pragma once

#include <string>
#include <filesystem>

namespace Quasar
{
    enum class ShaderCacheAPI
    {
        None = 0,
        OpenGL,
        Vulkan
    };
    
    class FileSystem
    {
    private:
        static std::filesystem::path s_CurrentPath;

    public:
        static std::string getAbsolutePath(std::string_view filepath);
        static std::string getAssetPath(std::string_view assetPath); // should maybe be inside the asset manager later on

        static std::filesystem::path getShaderCacheDirectory(ShaderCacheAPI api);
        static void createShaderCacheDirectoryIfNeeded(ShaderCacheAPI api);

        // Look into making these constexpr when the c++20 constexpr std::string standard gets implemented by mayor compilers (not only msvc)
    };

} // namespace Quasar
