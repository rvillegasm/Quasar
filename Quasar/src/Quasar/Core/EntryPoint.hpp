#pragma once
#include "Quasar/Core/Core.hpp"
#include "Quasar/Core/Log.hpp"
#include "Quasar/Core/Application.hpp"

#include "Quasar/Debug/Instrumentor.hpp"

#include <filesystem>
#include <iostream>

extern Quasar::Application *Quasar::createApplication();

int main(int argc, char **argv)
{
    Quasar::Log::init();

    std::string currentWorkingPath = std::filesystem::current_path();
    std::string startupSessionPath(currentWorkingPath);
    std::string runtimeSessionPath(currentWorkingPath);
    std::string shutdownSessionPath(currentWorkingPath);

    QS_PROFILE_BEGIN_SESSION("Startup", startupSessionPath.append("/QuasarProfile-Startup.json"));
    auto app = Quasar::createApplication();
    QS_PROFILE_END_SESSION();

    QS_PROFILE_BEGIN_SESSION("Runtime", runtimeSessionPath.append("/QuasarProfile-Runtime.json"));
    app->run();
    QS_PROFILE_END_SESSION();

    QS_PROFILE_BEGIN_SESSION("Shutdown", shutdownSessionPath.append("/QuasarProfile-Shutdown.json"));
    delete app;
    QS_PROFILE_END_SESSION();
}