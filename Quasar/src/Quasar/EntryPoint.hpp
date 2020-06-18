#pragma once
#include "Application.hpp"

extern Quasar::Application *Quasar::createApplication();

int main(int argc, char **argv)
{
    Quasar::Log::init();
    QS_CORE_WARN("Initialized Log");
    int a = 5;
    QS_INFO("Hello! Var={0}", a);

    auto app = Quasar::createApplication();
    app->run();
    delete app;
}