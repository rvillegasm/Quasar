#pragma once

namespace Quasar
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        void run();
    };

    // To be defined in the CLIENT
    Application *createApplication();
} // namespace Quasar