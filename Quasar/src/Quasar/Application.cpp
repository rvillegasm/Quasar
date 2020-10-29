#include "Application.hpp"

#include "Quasar/Core.hpp"
#include "Quasar/Log.hpp"

namespace Quasar
{

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

    Application *Application::s_Instance = nullptr;

    Application::Application()
    {
        QS_CORE_ASSERT(!s_Instance, "An Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::create());
        m_Window->setEventCallback(BIND_EVENT_FN(Application::onEvent));

        m_ImGuiLayer = new ImGuiLayer();
        pushOverlay(m_ImGuiLayer);
    }

    Application::~Application() 
    {
    }

    void Application::pushLayer(Layer *layer)
    {
        m_LayerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(Layer *overlay)
    {
        m_LayerStack.pushOverlay(overlay);
        overlay->onAttach();
    }

    void Application::onEvent(Event &e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClosed));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
        {
            (*--it)->onEvent(e);
            if (e.isHandled())
            {
                break;
            }
        }
    }

    void Application::run()
    {
        while (m_Running)
        {
            for (Layer *layer : m_LayerStack)
            {
                layer->onUpdate();
            }

            m_ImGuiLayer->begin();
            for (Layer *layer : m_LayerStack)
            {
                layer->onImGuiRender();
            }
            m_ImGuiLayer->end();

            m_Window->onUpdate();
        }
    }

    bool Application::onWindowClosed(WindowCloseEvent &e)
    {
        m_Running = false;
        return true;
    }

} // namespace Quasar