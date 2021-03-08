#pragma once

#include <string>
#include <utility>
#include <optional>

namespace Quasar
{
    
    class FileDialogs
    {
    public:
        static std::optional<std::string> openFile(const std::pair<std::string, std::string> &filter); // "All Files", "*"
        static std::optional<std::string> saveFile(const std::pair<std::string, std::string> &filter);
    };

} // namespace Quasar
