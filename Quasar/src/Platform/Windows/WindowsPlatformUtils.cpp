#include <Windows.h>
#include "Quasar/Utils/PlatformUtils.hpp"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Quasar/Core/Application.hpp"

namespace Quasar
{

    std::string FileDialogs::openFile(const std::pair<std::string, std::string> &filter)
    {
        std::string str = filter.first + '\0' + filter.second + '\0';
        const char *filterStr = str.c_str();

        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        CHAR currentDir[256] = { 0 };
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow *)Application::get().getWindow().getNativeWindow());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        if (GetCurrentDirectoryA(256, currentDir))
        {
            ofn.lpstrInitialDir = currentDir;
        }
        ofn.lpstrFilter = filterStr;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn) == TRUE)
        {
            return ofn.lpstrFile;
        }
        return "";
    }

    std::string FileDialogs::saveFile(const std::pair<std::string, std::string> &filter)
    {
        std::string str = filter.first + '\0' + filter.second + '\0';
        const char *filterStr = str.c_str();

        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        CHAR currentDir[256] = { 0 };
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow *)Application::get().getWindow().getNativeWindow());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        if (GetCurrentDirectoryA(256, currentDir))
        {
            ofn.lpstrInitialDir = currentDir;
        }
        ofn.lpstrFilter = filterStr;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

        // Sets the default extension by extracting it from the filter
        ofn.lpstrDefExt = strchr(filterStr, '\0') + 1;

        if (GetSaveFileNameA(&ofn) == TRUE)
        {
            return ofn.lpstrFile;
        }
        return "";
    }

} // namespace Quasar
