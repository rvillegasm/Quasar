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

    class Application
    {
    private:
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
        Application(const std::string &name = "Quasar App");
        virtual ~Application();

        void onEvent(Event &e);

        void pushLayer(Layer *layer);
        void pushOverlay(Layer *overlay);

        Window &getWindow() { return *m_Window; }

        void close();

        ImGuiLayer *getImGuiLayer() { return m_ImGuiLayer; }
        
        static Application &get() { return *s_Instance; }
    };

    // To be defined in the CLIENT
    Application *createApplication();

} // namespace Quasar