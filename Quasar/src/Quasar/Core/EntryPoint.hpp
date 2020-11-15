#pragma once
#include "Quasar/Core/Core.hpp"
#include "Quasar/Core/Log.hpp"
#include "Quasar/Core/Application.hpp"

extern Quasar::Application *Quasar::createApplication();

int main(int argc, char **argv)
{
    Quasar::Log::init();
    QS_CORE_WARN("Initialized Log");

    auto app = Quasar::createApplication();
    app->run();
    delete app;
}