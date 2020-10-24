#pragma once
#include "Application.hpp"

extern Quasar::Application *Quasar::createApplication();

int main(int argc, char **argv)
{
    Quasar::Log::init();
    QS_CORE_WARN("Initialized Log");

    auto app = Quasar::createApplication();
    app->run();
    delete app;
}