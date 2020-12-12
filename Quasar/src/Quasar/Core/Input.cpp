#include "Input.hpp"

#ifdef QS_PLATFORM_LINUX
    #include "Platform/Linux/LinuxInput.hpp"
#endif

namespace Quasar
{

    Scope<Input> Input::s_Instance = Input::create();

    Scope<Input> Input::create()
    {
    #ifdef QS_PLATFORM_LINUX
        return createScope<LinuxInput>();
    #else
        QS_CORE_ASSERT(false, "Unkown platform!");
        return nullptr;
    #endif
    }

} // namespace Quasar
