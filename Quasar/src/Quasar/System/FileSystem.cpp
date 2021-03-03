#include "FileSystem.hpp"

namespace Quasar
{

    std::filesystem::path FileSystem::s_CurrentPath = std::filesystem::current_path();
    
    std::string FileSystem::getAbsolutePath(std::string_view filepath)
    {
        return (s_CurrentPath / filepath);
    }
    
    std::string FileSystem::getAssetPath(std::string_view assetPath) 
    {
        return (s_CurrentPath / "assets" / assetPath);
    }

} // namespace Quasar
