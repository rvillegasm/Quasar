#pragma once

#include "Quasar/Core/Log.hpp"
#include "Quasar/Core/PlatformDetection.hpp"

#include <memory>
 
#ifdef QS_DEBUG
    #if defined(QS_PLATFORM_WINDOWS)
        #define QS_DEBUGBREAK() __debugbreak()
    #elif defined(QS_PLATFORM_LINUX)
        #include <signal.h>
		#define QS_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform does not support debugbreak yet!"
    #endif
    #define QS_ENABLE_ASSERTS
#else
    #define QS_DEBUGBREAK()
#endif

#ifdef QS_ENABLE_ASSERTS
    #include <cstdlib>
    #define QS_ASSERT(x, ...)                                   \
        {                                                       \
            if (!(x))                                           \
            {                                                   \
                QS_ERROR("Assertion failed: {0}", __VA_ARGS__); \
                QS_DEBUGBREAK();                                \
                std::exit(EXIT_FAILURE);                        \
            }                                                   \
        }
    #define QS_CORE_ASSERT(x, ...)                                   \
        {                                                            \
            if (!(x))                                                \
            {                                                        \
                QS_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); \
                QS_DEBUGBREAK();                                     \
                std::exit(EXIT_FAILURE);                             \
            }                                                        \
        }
#else
    #define QS_ASSERT(x, ...)
    #define QS_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define QS_BIND_EVENT_FN(fn) [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Quasar
{
    
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> createScope(Args &&...args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> createRef(Args &&...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

} // namespace Quasar
