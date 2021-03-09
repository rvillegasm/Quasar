#pragma once

#include "Quasar/System/DllLoader.hpp"

#include <Windows.h>

namespace Quasar
{
    
    template<typename T>
    class WindowsDllLoader : public DllLoader<T>
    {
    private:
        HMODULE m_Handle;
        std::string m_Filepath;

    protected:
        void openDll() override;
        void closeDll() override;

    public:
        WindowsDllLoader(const std::string &filepath);
        ~WindowsDllLoader();

        Ref<T> getClassInstance(const std::string &allocClassSymbol = "create", const std::string &destroyClassSymbol = "destroy") override;
    };

} // namespace Quasar
