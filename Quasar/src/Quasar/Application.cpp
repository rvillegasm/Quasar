#include "Application.hpp"

#include "Quasar/Log.hpp"
#include "Quasar/Events/ApplicationEvent.hpp"

namespace Quasar
{

    Application::Application() {}

    Application::~Application() {}

    void Application::run()
    {
        WindowResizeEvent e(1280, 720);
        if (e.isInCategory(EventCategoryApplication))
        {
            QS_TRACE(e);
        }
        if (e.isInCategory(EventCategoryInput))
        {
            QS_TRACE(e);
        }

        while (true);
    }

} // namespace Quasar