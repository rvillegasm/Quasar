#pragma once

#include "Quasar/Window.hpp"
#include "Quasar/LayerStack.hpp"
#include "Quasar/Events/Event.hpp"
#include "Quasar/Events/ApplicationEvent.hpp"

#include "Quasar/ImGui/ImGuiLayer.hpp"

#include <memory>

namespace Quasar
{

    class Application
    {
    private:
        std::unique_ptr<Window> m_Window;
        ImGuiLayer *m_ImGuiLayer;
        bool m_Running = true;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;

        static Application *s_Instance;

        bool onWindowClosed(WindowCloseEvent &e);

    public:
        Application();
        virtual ~Application();

        void run();

        void onEvent(Event &e);

        void pushLayer(Layer *layer);
        void pushOverlay(Layer *overlay);

        inline Window &getWindow() { return *m_Window; }
        
        inline static Application &get() { return *s_Instance; }
    };

    // To be defined in the CLIENT
    Application *createApplication();

} // namespace Quasar