#include "Quasar/Utils/PlatformUtils.hpp"

#include <portable-file-dialogs.h>

namespace Quasar
{
    // TODO: In the future, every platform will use native implementations

    std::string FileDialogs::openFile(const std::vector<std::string> &filters)
    {
        auto selectedFile = pfd::open_file("Open File", ".", filters).result();
        // if the user cancelled, the vector is empty.
        return selectedFile.empty() ? "" : selectedFile[0];
    }

    std::string FileDialogs::saveFile(const std::vector<std::string> &filters)
    {
        auto destinationFile = pfd::save_file("Save File", ".", filters).result();
        return destinationFile; // if the user cancelled, the string is empty.
    }

} // namespace Quasar
