#pragma once

#include "Quasar/Core/Log.hpp"

#include <memory>

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

    template<typename T>
    using Ref = std::shared_ptr<T>;

} // namespace Quasar
