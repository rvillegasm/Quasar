#pragma once

#include <filesystem>

namespace Quasar
{

    class ContentBrowserPanel
    {
    private:
        std::filesystem::path m_CurrentDirectory;
    public:
        ContentBrowserPanel();
        void onImGuiRender();
    };

}