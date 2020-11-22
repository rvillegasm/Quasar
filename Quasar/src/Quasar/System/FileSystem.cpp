#include "FileSystem.hpp"

#include <filesystem>

namespace Quasar
{

    FileSystem::FileSystem()
    {
        m_CurrentPath = std::filesystem::current_path();
    }
    
    FileSystem &FileSystem::get()
    {
        static FileSystem instance;
        return instance;
    }
    
    std::string FileSystem::getRealFilepath(std::string_view filepath) 
    {
        return (FileSystem::get().m_CurrentPath + '/').append(filepath);
    }

} // namespace Quasar
