#include "Application.hpp"

#include "Quasar/Log.hpp"
#include "Quasar/Events/ApplicationEvent.hpp"

namespace Quasar
{

    Application::Application()
    {
        m_Window = std::unique_ptr<Window>(Window::create());
    }

    Application::~Application() {}

    void Application::run()
    {
        while (m_Running)
        {
            m_Window->onUpdate();
        }
    }

} // namespace Quasar