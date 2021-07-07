#include "ContentBrowserPanel.hpp"

#include "imgui.h"

namespace Quasar
{
    // TODO: Change once the engine supports projects
    static const std::filesystem::path s_AssetsPath = "assets";

    ContentBrowserPanel::ContentBrowserPanel()
        : m_CurrentDirectory(s_AssetsPath)
    {
    }

    void ContentBrowserPanel::onImGuiRender()
    {
        ImGui::Begin("Content Browser");

        if (m_CurrentDirectory != s_AssetsPath)
        {
            if (ImGui::Button("<-"))
            {
                m_CurrentDirectory = m_CurrentDirectory.parent_path();
            }
        }

        for (auto &directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
        {
            const auto &path = directoryEntry.path();
            auto relativePath = std::filesystem::relative(path, s_AssetsPath);
            std::string filenameString = relativePath.filename().string();

            if (directoryEntry.is_directory())
            {
                if (ImGui::Button(filenameString.c_str()))
                {
                    m_CurrentDirectory /= path.filename();
                }
            }
            else
            {
                if (ImGui::Button(filenameString.c_str()))
                {
                }
            }
        }

        ImGui::End();
    }

}