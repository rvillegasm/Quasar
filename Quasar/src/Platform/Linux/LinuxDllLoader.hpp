#pragma once

#include "Quasar/System/DllLoader.hpp"

#include <dlfcn.h>

namespace Quasar
{
    
    template<typename T>
    class LinuxDllLoader : public DllLoader<T>
    {
    private:
        void *m_Handle;
        std::string m_Filepath;

    protected:
        void openDll() override;
        void closeDll() override;

    public:
        LinuxDllLoader(const std::string &filepath);
        ~LinuxDllLoader();

        Ref<T> getClassInstance(const std::string &allocClassSymbol = "create", const std::string &destroyClassSymbol = "destroy") override;
    };

} // namespace Quasar
