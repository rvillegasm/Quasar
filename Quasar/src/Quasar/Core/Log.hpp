#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <memory>

namespace Quasar
{

    class Log
    {
    public:
        static void init();

        static std::shared_ptr<spdlog::logger> &getCoreLogger() { return s_CoreLogger; }
        static std::shared_ptr<spdlog::logger> &getClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

} // namespace Quasar

// ----- Logging conveniency macros -----

// Core logger

#define QS_CORE_TRACE(...)    ::Quasar::Log::getCoreLogger()->trace(__VA_ARGS__);
#define QS_CORE_INFO(...)     ::Quasar::Log::getCoreLogger()->info(__VA_ARGS__);
#define QS_CORE_WARN(...)     ::Quasar::Log::getCoreLogger()->warn(__VA_ARGS__);
#define QS_CORE_ERROR(...)    ::Quasar::Log::getCoreLogger()->error(__VA_ARGS__);
#define QS_CORE_CRITICAL(...) ::Quasar::Log::getCoreLogger()->critical(__VA_ARGS__);

// Client logger

#define QS_TRACE(...)      ::Quasar::Log::getClientLogger()->trace(__VA_ARGS__);
#define QS_INFO(...)       ::Quasar::Log::getClientLogger()->info(__VA_ARGS__);
#define QS_WARN(...)       ::Quasar::Log::getClientLogger()->warn(__VA_ARGS__);
#define QS_ERROR(...)      ::Quasar::Log::getClientLogger()->error(__VA_ARGS__);
#define QS_CRITICAL(...)   ::Quasar::Log::getClientLogger()->critical(__VA_ARGS__);

// --------------------------------------