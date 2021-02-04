#pragma once

#include <string>
#include <filesystem>

namespace Quasar
{
    
    class FileSystem
    {
    private:
        static std::filesystem::path s_CurrentPath;

    public:
        static std::string getAbsolutePath(std::string_view filepath);
    };

} // namespace Quasar
