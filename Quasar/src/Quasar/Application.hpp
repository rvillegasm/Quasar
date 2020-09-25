#pragma once

#include "Quasar/Window.hpp"
#include "Quasar/Events/Event.hpp"
#include "Quasar/Events/ApplicationEvent.hpp"

#include <memory>

namespace Quasar
{

    class Application
    {
    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;

        bool onWindowClosed(WindowCloseEvent &e);

    public:
        Application();
        virtual ~Application();

        void run();

        void onEvent(Event &e);
    };

    // To be defined in the CLIENT
    Application *createApplication();

} // namespace Quasar