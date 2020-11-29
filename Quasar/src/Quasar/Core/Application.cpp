#include "Application.hpp"

#include "Quasar/Core/Core.hpp"
#include "Quasar/Core/Log.hpp"
#include "Quasar/Core/Timestep.hpp"

#include "Quasar/Renderer/Renderer.hpp"

#include "Quasar/Debug/Instrumentor.hpp"

#include <GLFW/glfw3.h> // Temporary, here just to get the time

namespace Quasar
{

    Application *Application::s_Instance = nullptr;

    Application::Application()
    {
        QS_PROFILE_FUNCTION();

        QS_CORE_ASSERT(!s_Instance, "An Application already exists!");
        s_Instance = this;

        m_Window = Window::create();
        m_Window->setEventCallback(QS_BIND_EVENT_FN(Application::onEvent));

        Renderer::init();

        m_ImGuiLayer = new ImGuiLayer();
        pushOverlay(m_ImGuiLayer);
    }

    Application::~Application() 
    {
        QS_PROFILE_FUNCTION();

        Renderer::shutdown();
    }

    void Application::pushLayer(Layer *layer)
    {
        QS_PROFILE_FUNCTION();

        m_LayerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(Layer *overlay)
    {
        QS_PROFILE_FUNCTION();

        m_LayerStack.pushOverlay(overlay);
        overlay->onAttach();
    }

    void Application::onEvent(Event &e)
    {
        QS_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(QS_BIND_EVENT_FN(Application::onWindowClose));
        dispatcher.dispatch<WindowResizeEvent>(QS_BIND_EVENT_FN(Application::onWindowResize));

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        {
            (*it)->onEvent(e);
            if (e.isHandled())
            {
                break;
            }
        }
    }

    void Application::run()
    {
        QS_PROFILE_FUNCTION();

        while (m_Running)
        {
            QS_PROFILE_SCOPE("RunLoop Iteration");

            float time = (float)glfwGetTime(); // Move to Platform::GetTime() or something like that
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            if (!m_Minimized)
            {
                {
                    QS_PROFILE_SCOPE("LayerStack onUpdate");

                    for (Layer *layer : m_LayerStack)
                    {
                        layer->onUpdate(timestep);
                    }
                }

                m_ImGuiLayer->begin();
                {
                    QS_PROFILE_SCOPE("LayerStack onImGuiRender");

                    for (Layer *layer : m_LayerStack)
                    {
                        layer->onImGuiRender();
                    }
                }
                m_ImGuiLayer->end();
            }


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
        QS_PROFILE_FUNCTION();

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