#include "Window.hpp"

#ifdef QS_PLATFORM_LINUX
    #include "Platform/Linux/LinuxWindow.hpp"
#endif

namespace Quasar
{
    
    Scope<Window> Window::create(const WindowProps &props)
    {
    #ifdef QS_PLATFORM_LINUX
        return createScope<LinuxWindow>(props);
    #else
        QS_CORE_ASSERT(false, "Unkown platform!");
        return nullptr;
    #endif

    }

} // namespace Quasar
