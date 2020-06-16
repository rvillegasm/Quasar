#pragma once
#include "Application.hpp"

extern Quasar::Application *Quasar::createApplication();

int main(int argc, char **argv)
{
    auto app = Quasar::createApplication();
    app->run();
    delete app;
}