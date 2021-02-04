#pragma once

#include <string>
#include <filesystem>

namespace Quasar
{
    
    class FileSystem
    {
    private:
        static std::filesystem::path s_CurrentPath;
        static std::filesystem::path s_LoggingDirectoryPath;

    public:
        static std::string getAbsolutePath(std::string_view filepath);
    };

} // namespace Quasar
