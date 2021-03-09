#pragma once

#include "Quasar/Core/Base.hpp"

#include <string>

namespace Quasar
{
    template<typename T>
    class DllLoader
    {
    protected:
        virtual void openDll() = 0; // must be called on the constructor
        virtual void closeDll() = 0; // must be called on the destructor

    public:
        virtual ~DllLoader() = default;

        /**
         * @brief Get a Class T Instance from the DLL.
         * 
         * The instance is only valid during the DllLoader's lifetime.
         * 
         * @param allocClassSymbol exported dll symbol used to create an instance of class T (default = "create").
         * @param destroyClassSymbol exported dll symbol used to destoy an instance of class T (deafult = "destroy").
         * @return Reference to the instance.
         */
        virtual Ref<T> getClassInstance(const std::string &allocClassSymbol = "create", const std::string &destroyClassSymbol = "destroy") = 0;

        static Scope<DllLoader<T>> create(const std::string &filepath);
    };

} // namespace Quasar
