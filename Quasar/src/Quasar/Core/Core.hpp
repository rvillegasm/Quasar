#pragma once

#include "Quasar/Core/Log.hpp"

#include <memory>


// Automatic platform detection
#ifdef _WIN32
    #ifdef _WIN64
        /* Windows x64 */
        #define QS_PLATFORM_WINDOWS
    #else
        /* Windows x86 */
        #define QS_PLATFORM_WINDOWS
    #endif
    #error "Windows is not supported yet!"

#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    /* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
    #if TARGET_IPHONE_SIMULATOR == 1
        #error "IOS simulator is not supported!"
    #elif TARGET_OS_IPHONE == 1
		#define QS_PLATFORM_IOS
		#error "IOS is not supported yet!"
	#elif TARGET_OS_MAC == 1
		#define QS_PLATFORM_MACOS
		#error "MacOS is not supported yet!"
    #else
        #error "Unknown Apple platform!"
    #endif

/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
    #define QS_PLATFORM_ANDROID
    #error "Android is not supported yet!"

#elif defined(__linux__)
    #define QS_PLATFORM_LINUX

#else
    #error "Could not detect platform!"

#endif
// End of automatic platform detection

#ifdef QS_ENABLE_ASSERTS
    #include <cstdlib>
    #define QS_ASSERT(x, ...)                                   \
        {                                                       \
            if (!(x))                                           \
            {                                                   \
                QS_ERROR("Assertion failed: {0}", __VA_ARGS__); \
                std::exit(EXIT_FAILURE);                        \
            }                                                   \
        }
    #define QS_CORE_ASSERT(x, ...)                                   \
        {                                                            \
            if (!(x))                                                \
            {                                                        \
                QS_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); \
                std::exit(EXIT_FAILURE);                             \
            }                                                        \
        }
#else
    #define QS_ASSERT(x, ...)
    #define QS_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define QS_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

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
