#include "Application.hpp"

#include "Quasar/Core/Core.hpp"
#include "Quasar/Core/Log.hpp"
#include "Quasar/Core/Timestep.hpp"

#include "Quasar/Renderer/Renderer.hpp"

#include <GLFW/glfw3.h> // Temporary, here just to get the time

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

        Renderer::init();

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
        dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));
        dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::onWindowResize));

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
            float time = (float)glfwGetTime(); // Move to Platform::GetTime() or something like that
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            if (!m_Minimized)
            {
                for (Layer *layer : m_LayerStack)
                {
                    layer->onUpdate(timestep);
                }
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

    bool Application::onWindowClose(WindowCloseEvent &e)
    {
        m_Running = false;
        return true;
    }

    bool Application::onWindowResize(WindowResizeEvent &e)
    {
        if (e.getWidth() == 0 || e.getHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        Renderer::onWindowResize(e.getWidth(), e.getHeight());

        return false;
    }

} // namespace Quasar