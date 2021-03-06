#pragma once

#include "Quasar/Core/Window.hpp"
#include "Quasar/Core/LayerStack.hpp"
#include "Quasar/Events/Event.hpp"
#include "Quasar/Events/ApplicationEvent.hpp"

#include "Quasar/ImGui/ImGuiLayer.hpp"

#include <memory>
#include <string>

int main(int argc, char **argv);

namespace Quasar
{

    struct ApplicationCommandLineArgs
    {
        int count = 0;
        char **args = nullptr;

        const char *operator[](int index) const
        {
            QS_CORE_ASSERT(index < count);
            return args[index];
        }
    };

    class Application
    {
    private:
        ApplicationCommandLineArgs m_CommandLineArgs;
        Scope<Window> m_Window;
        ImGuiLayer *m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;

        static Application *s_Instance;

        void run();

        bool onWindowClose(WindowCloseEvent &e);
        bool onWindowResize(WindowResizeEvent &e);

        friend int ::main(int argc, char **argv);

    public:
        Application(const std::string &name = "Quasar App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
        virtual ~Application();

        void onEvent(Event &e);

        void pushLayer(Layer *layer);
        void pushOverlay(Layer *overlay);

        Window &getWindow() { return *m_Window; }

        void close();

        ImGuiLayer *getImGuiLayer() { return m_ImGuiLayer; }

        ApplicationCommandLineArgs getCommandLineArgs() const { return m_CommandLineArgs; };

        static Application &get() { return *s_Instance; }
    };

    // To be defined in the CLIENT
    Application *createApplication(ApplicationCommandLineArgs args);

} // namespace Quasar