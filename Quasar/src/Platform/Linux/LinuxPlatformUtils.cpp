#include "Quasar/Utils/PlatformUtils.hpp"

#include <portable-file-dialogs.h>

namespace Quasar
{
    // TODO: In the future, every platform will use native implementations

    std::optional<std::string> FileDialogs::openFile(const std::vector<std::string> &filters)
    {
        auto selectedFile = pfd::open_file("Open File", ".", filters).result();
        // if the user cancelled, the vector is empty.
        return !selectedFile.empty() ? std::make_optional(selectedFile[0]) : std::nullopt;
    }

    std::optional<std::string> FileDialogs::saveFile(const std::vector<std::string> &filters)
    {
        auto destinationFile = pfd::save_file("Save File", ".qscene", filters).result();
        // if the user cancelled, the string is empty.
        return !destinationFile.empty() ? std::make_optional(destinationFile) : std::nullopt;
    }

} // namespace Quasar
