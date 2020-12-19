#pragma once

#include "Quasar/Core/Core.hpp"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <chrono>
#include <algorithm>
#include <thread>

namespace Quasar
{

    using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

    struct ProfileResult
    {
        std::string name;

        FloatingPointMicroseconds start;
        std::chrono::microseconds elapsedTime;

        std::thread::id threadID;
    };

    struct InstrumentationSession
    {
        std::string name;
    };

    class Instrumentor
    {
    private:
        std::mutex m_Mutex;
        InstrumentationSession *m_CurrentSession;
        std::ofstream m_OutputStream;

    public:
        Instrumentor()
            : m_CurrentSession(nullptr)
        {
        }

        void beginSession(const std::string &name, const std::string &filepath = "results.json")
        {
            std::lock_guard lock(m_Mutex);
            if (m_CurrentSession)
            {
                if (Log::getCoreLogger())
                {
                    QS_CORE_ERROR("Instrumentor::beginSession('{0}') when session '{1}' already open!", name, m_CurrentSession->name);
                }
                internalEndSession();
            }

            m_OutputStream.open(filepath);

            if (m_OutputStream.is_open())
            {
                m_CurrentSession = new InstrumentationSession({name});
                writeHeader();
            }
            else
            {
                if (Log::getCoreLogger())
                {
                    QS_CORE_ERROR("Instrumentor could not open output file '{0}'!", filepath);
                }
            }

        }

        void endSession()
        {
            std::lock_guard lock(m_Mutex);
            internalEndSession();
        }

        void writeProfile(const ProfileResult &result)
        {
            std::stringstream json;

            std::string name = result.name;
            std::replace(name.begin(), name.end(), '"', '\'');

            json << std::setprecision(3) << std::fixed;
            json << ",{";
            json << "\"cat\":\"function\",";
            json << "\"dur\":" << (result.elapsedTime.count()) << ',';
            json << "\"name\":\"" << name << "\",";
            json << "\"ph\":\"X\",";
            json << "\"pid\":0,";
            json << "\"tid\":" << result.threadID << ',';
            json << "\"ts\":" << result.start.count();
            json << "}";

            std::lock_guard lock(m_Mutex);
            if (m_CurrentSession)
            {
                m_OutputStream << json.str();
                m_OutputStream.flush();
            }

        }

        static Instrumentor &get()
        {
            static Instrumentor instance;
            return instance;
        }

    private:
        void writeHeader()
        {
            m_OutputStream << "{\"otherData\": {}, \"traceEvents\":[{}";
            m_OutputStream.flush();
        }

        void writeFooter()
        {
            m_OutputStream << "]}\n";
            m_OutputStream.flush();
        }

        void internalEndSession()
        {
            if (m_CurrentSession)
            {
                writeFooter();
                m_OutputStream.close();
                delete m_CurrentSession;
                m_CurrentSession = nullptr;
            }
        }

    };

    class InstrumentationTimer
    {
    private:
        const char *m_Name;
        std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
        bool m_Stopped;

    public:
        InstrumentationTimer(const char *name)
            : m_Name(name), m_Stopped(false)
        {
            m_StartTimepoint = std::chrono::steady_clock::now();
        }

        ~InstrumentationTimer()
        {
            if (!m_Stopped)
            {
                stop();
            }
        }

        void stop()
        {
            auto endTimepoint = std::chrono::steady_clock::now();
            auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };

            auto elapsedTime =
                std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch()
                - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

            Instrumentor::get().writeProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });

            m_Stopped = true;
        }
    };

} // namespace Quasar

#if QS_PROFILE
    #if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
        #define QS_FUNC_SIG __PRETTY_FUNCTION__
    #elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define QS_FUNC_SIG __PRETTY_FUNCTION__
    #elif defined(__FUNCSIG__)
		#define QS_FUNC_SIG __FUNCSIG__
    #elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define QS_FUNC_SIG __FUNCTION__
    #elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define QS_FUNC_SIG __FUNC__
    #elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define QS_FUNC_SIG __func__
    #elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define QS_FUNC_SIG __func__
    #else
		#define QS_FUNC_SIG "QS_FUNC_SIG unknown!"
	#endif

    #define QS_PROFILE_BEGIN_SESSION(name, filepath) ::Quasar::Instrumentor::get().beginSession(name, filepath)
    #define QS_PROFILE_END_SESSION() ::Quasar::Instrumentor::get().endSession()
    #define QS_PROFILE_SCOPE(name) ::Quasar::InstrumentationTimer timer##__LINE__(name)
    #define QS_PROFILE_FUNCTION() QS_PROFILE_SCOPE(QS_FUNC_SIG)
    
#else
    #define QS_PROFILE_BEGIN_SESSION(name, filepath)
    #define QS_PROFILE_END_SESSION()
    #define QS_PROFILE_SCOPE(name)
    #define QS_PROFILE_FUNCTION()
#endif