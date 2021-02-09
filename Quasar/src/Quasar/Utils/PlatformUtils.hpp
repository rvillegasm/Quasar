#pragma once

#include <string>
#include <vector>

namespace Quasar
{
    
    class FileDialogs
    {
    public:
        static std::string openFile(const std::vector<std::string> &filters);
        static std::string saveFile(const std::vector<std::string> &filters);
    };

} // namespace Quasar
