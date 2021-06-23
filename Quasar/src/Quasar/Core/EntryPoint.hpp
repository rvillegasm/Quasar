#pragma once
#include "Quasar/Core/Base.hpp"
#include "Quasar/Core/Log.hpp"
#include "Quasar/Core/Application.hpp"

#include "Quasar/System/FileSystem.hpp"

#include "Quasar/Debug/Instrumentor.hpp"

extern Quasar::Application *Quasar::createApplication(ApplicationCommandLineArgs args);

int main(int argc, char **argv)
{
    Quasar::Log::init();

    QS_PROFILE_BEGIN_SESSION("Startup", Quasar::FileSystem::getAbsolutePath("QuasarProfile-Startup.json"));
    auto app = Quasar::createApplication({ argc, argv });
    QS_PROFILE_END_SESSION();

    QS_PROFILE_BEGIN_SESSION("Runtime", Quasar::FileSystem::getAbsolutePath("QuasarProfile-Runtime.json"));
    app->run();
    QS_PROFILE_END_SESSION();

    QS_PROFILE_BEGIN_SESSION("Shutdown", Quasar::FileSystem::getAbsolutePath("QuasarProfile-Shutdown.json"));
    delete app;
    QS_PROFILE_END_SESSION();
}