#pragma once

#include "Quasar/Core/Log.hpp"

#include <memory>
#include <filesystem>

#ifdef QS_DEBUG
    #if defined(QS_PLATFORM_WINDOWS)
        #define QS_DEBUGBREAK() __debugbreak()
    #elif defined(QS_PLATFORM_LINUX) || defined(QS_PLATFORM_MACOS)
        #include <signal.h>
		#define QS_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform does not support debugbreak yet!"
    #endif
    #define QS_ENABLE_ASSERTS
#else
    #define QS_DEBUGBREAK()
#endif

#define QS_EXPAND_MACRO(x) x
#define QS_STRINGIFY_MACRO(x) #x

#ifdef QS_ENABLE_ASSERTS
    #define QS_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if (!(check)) { QS##type##ERROR(msg, __VA_ARGS__); QS_DEBUGBREAK(); } }
    
    #define QS_INTERNAL_ASSERT_WITH_MSG(type, check, ...) QS_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
    #define QS_INTERNAL_ASSERT_NO_MSG(type, check) QS_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", QS_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

    #define QS_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
    #define QS_INTERNAL_ASSERT_GET_MACRO(...) QS_EXPAND_MACRO( QS_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, QS_INTERNAL_ASSERT_WITH_MSG, QS_INTERNAL_ASSERT_NO_MSG) )

    // Accepts the condition first, and then an optional message parameter
    #define QS_ASSERT(...) QS_EXPAND_MACRO( QS_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
    #define QS_CORE_ASSERT(...) QS_EXPAND_MACRO( QS_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
    #define QS_ASSERT(...)
    #define QS_CORE_ASSERT(...)
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
