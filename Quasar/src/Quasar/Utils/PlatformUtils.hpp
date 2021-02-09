#pragma once

#include <string>
#include <vector>
#include <optional>

namespace Quasar
{
    
    class FileDialogs
    {
    public:
        static std::optional<std::string> openFile(const std::vector<std::string> &filters);
        static std::optional<std::string> saveFile(const std::vector<std::string> &filters);
    };

} // namespace Quasar
