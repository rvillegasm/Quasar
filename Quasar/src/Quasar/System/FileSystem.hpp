#pragma once

#include <string>

namespace Quasar
{
    
    class FileSystem
    {
    private:
        std::string m_CurrentPath;

        static FileSystem &get();

    public:
        FileSystem();
        FileSystem(const FileSystem&) = delete;
        FileSystem &operator=(const FileSystem&) = delete;

        virtual ~FileSystem() = default;

        static std::string getRealFilepath(std::string_view filepath);
    };

} // namespace Quasar
