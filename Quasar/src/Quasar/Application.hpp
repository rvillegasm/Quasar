#pragma once

#include <memory>

#include "Quasar/Window.hpp"

namespace Quasar
{

    class Application
    {
    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;

    public:
        Application();
        virtual ~Application();

        void run();
    };

    // To be defined in the CLIENT
    Application *createApplication();

} // namespace Quasar