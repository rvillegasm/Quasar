#include "WindowsDllLoader.hpp"

#include "Quasar/Scene/ScriptableEntity.hpp"

namespace Quasar
{
    
    template<typename T>
    Scope<DllLoader<T>> DllLoader<T>::create(const std::string &filepath)
    {
        return createScope<WindowsDllLoader<T>>(filepath);
    }

    template<typename T>
    WindowsDllLoader<T>::WindowsDllLoader(const std::string &filepath) 
        : m_Filepath(filepath)
    {
        openDll();
    }
    
    template<typename T>
    WindowsDllLoader<T>::~WindowsDllLoader() 
    {
        closeDll();
    }
    
    template<typename T>
    Ref<T> WindowsDllLoader<T>::getClassInstance(const std::string &allocClassSymbol, const std::string &destroyClassSymbol)
    {
        using AllocClassFunc = T *(*)(); // typedef T *(*AllocClassFunc)()
        using DestroyClassFunc = void (*)(T *); // typedef void (*DestroyClassFunc)()

        auto allocFunc = reinterpret_cast<AllocClassFunc>(GetProcAddress(m_Handle, allocClassSymbol.c_str()));
        auto destroyFunc = reinterpret_cast<DestroyClassFunc>(GetProcAddress(m_Handle, destroyClassSymbol.c_str()));

        if (!allocFunc || !destroyFunc)
        {
            closeDll();
            QS_CORE_ASSERT(false, "Failed to find alloc or destroy symbols in Dll!");
        }
        // Use Ref's constructor that specifies a destroy function
        // Note that since the Dll is closed upon the DllLoader's destruction,
        // the Ref returned is only going tho be valid while the DllLoader is alive.
        return Ref<T>(allocFunc(), [destroyFunc](T *pointer) { destroyFunc(pointer); });
    }

    template<typename T>
    void WindowsDllLoader<T>::openDll()
    {
        m_Handle = LoadLibrary(m_Filepath.c_str());
        QS_CORE_ASSERT(m_Handle, "Failed to load Dll!");
    }
    
    template<typename T>
    void WindowsDllLoader<T>::closeDll()
    {
        int closed = FreeLibrary(m_Handle);
        QS_CORE_ASSERT(closed != 0, "Failed to close Dll!");
    }

    // Since the templates are specified in the translation units,
    // template specialization is needed for the right types to be
    // compiled and available for external use.
    // If Another type is needed for the DllLoader, put it down here
    template Scope<DllLoader<ScriptableEntity>> DllLoader<ScriptableEntity>::create(const std::string &filepath);
    template WindowsDllLoader<ScriptableEntity>::WindowsDllLoader(const std::string &filepath);
    template WindowsDllLoader<ScriptableEntity>::~WindowsDllLoader(); 
    template Ref<ScriptableEntity> WindowsDllLoader<ScriptableEntity>::getClassInstance(const std::string &allocClassSymbol, const std::string &destroyClassSymbol);
    template void WindowsDllLoader<ScriptableEntity>::closeDll();
    template void WindowsDllLoader<ScriptableEntity>::openDll();

} // namespace Quasar
