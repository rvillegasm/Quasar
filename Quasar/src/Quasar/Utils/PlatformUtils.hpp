#pragma once

#include <string>
#include <utility>

namespace Quasar
{
    
    class FileDialogs
    {
    public:
        static std::string openFile(const std::pair<std::string, std::string> &filter); // "All Files", "*"
        static std::string saveFile(const std::pair<std::string, std::string> &filter);
    };

} // namespace Quasar
