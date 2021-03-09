#include "LinuxDllLoader.hpp"

#include "Quasar/Scene/ScriptableEntity.hpp"

namespace Quasar
{
    
    template<typename T>
    Scope<DllLoader<T>> DllLoader<T>::create(const std::string &filepath)
    {
        return createScope<LinuxDllLoader<T>>(filepath);
    }

    template<typename T>
    LinuxDllLoader<T>::LinuxDllLoader(const std::string &filepath) 
        : m_Filepath(filepath)
    {
        openDll();
    }
    
    template<typename T>
    LinuxDllLoader<T>::~LinuxDllLoader() 
    {
        closeDll();
    }

    template<typename T>
    Ref<T> LinuxDllLoader<T>::getClassInstance(const std::string &allocClassSymbol, const std::string &destroyClassSymbol) 
    {
        using AllocClassFunc = T *(*)(); // typedef T *(*AllocClassFunc)()
        using DestroyClassFunc = void (*)(T *); // typedef void (*DestroyClassFunc)()

        auto allocFunc = reinterpret_cast<AllocClassFunc>(dlsym(m_Handle, allocClassSymbol.c_str()));
        auto destroyFunc = reinterpret_cast<DestroyClassFunc>(dlsym(m_Handle, destroyClassSymbol.c_str()));

        if (!allocFunc || !destroyFunc)
        {
            closeDll();
            QS_CORE_ASSERT(false, dlerror());
        }
        // Use Ref's constructor that sepcifies a destroy function
        // Note that since the Dll is closed upon the DllLoader's destruction,
        // the Ref returned is only going tho be valid while the DllLoader is alive.
        return Ref<T>(allocFunc(), [destroyFunc](T *pointer) { destroyFunc(pointer); });
    }

    template<typename T>
    void LinuxDllLoader<T>::openDll()
    {
        m_Handle = dlopen(m_Filepath.c_str(), RTLD_NOW | RTLD_LAZY);
        QS_CORE_ASSERT(m_Handle, dlerror());
    }
    
    template<typename T>
    void LinuxDllLoader<T>::closeDll() 
    {
        int closed = dlclose(m_Handle);
        QS_CORE_ASSERT(closed == 0, dlerror());
    }

    // Since the templates are specified in the translation units,
    // template specialization is needed for the right types to be
    // compiled and available for external use.
    // If Another type is needed for the DllLoader, put it down here
    template Scope<DllLoader<ScriptableEntity>> DllLoader<ScriptableEntity>::create(const std::string &filepath);
    template LinuxDllLoader<ScriptableEntity>::LinuxDllLoader(const std::string &filepath);
    template LinuxDllLoader<ScriptableEntity>::~LinuxDllLoader(); 
    template Ref<ScriptableEntity> LinuxDllLoader<ScriptableEntity>::getClassInstance(const std::string &allocClassSymbol, const std::string &destroyClassSymbol);
    template void LinuxDllLoader<ScriptableEntity>::closeDll();
    template void LinuxDllLoader<ScriptableEntity>::openDll();

} // namespace Quasar
