#include "LinuxWindow.hpp"

#include "Quasar/Core/Core.hpp"
#include "Quasar/Core/Log.hpp"

#include "Quasar/Events/ApplicationEvent.hpp"
#include "Quasar/Events/MouseEvent.hpp"
#include "Quasar/Events/KeyEvent.hpp"

#include "Platform/OpenGL/OpenGLContext.hpp"

#include"Quasar/Debug/Instrumentor.hpp"

namespace Quasar
{
    static uint8_t s_GLFWWindowCount = 0;

    static void GLFWErrorCallback(int error, const char *description)
    {
        QS_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    Scope<Window> Window::create(const WindowProps &props)
    {
        return createScope<LinuxWindow>(props);
    }

    LinuxWindow::LinuxWindow(const WindowProps &props)
    {
        QS_PROFILE_FUNCTION();

        init(props);
    }

    LinuxWindow::~LinuxWindow()
    {
        QS_PROFILE_FUNCTION();

        shutdown();
    }

    void LinuxWindow::init(const WindowProps &props)
    {
        QS_PROFILE_FUNCTION();

        m_Data.title = props.title;
        m_Data.width = props.width;
        m_Data.height = props.height;

        QS_CORE_INFO(
            "Creating window {0} ({1}, {2})",
            props.title, props.width, props.height
        );

        if (s_GLFWWindowCount == 0)
        {
            QS_PROFILE_SCOPE("glfwInit");
            int success = glfwInit();
            QS_CORE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
        }

        {
            QS_PROFILE_SCOPE("glfwCreateWindow");
            m_Window = glfwCreateWindow(
                (int) props.width,
                (int) props.height,
                m_Data.title.c_str(),
                nullptr,
                nullptr
            );
            s_GLFWWindowCount++;
        }

        m_Context = GraphicsContext::create(m_Window);
        m_Context->init();

        glfwSetWindowUserPointer(m_Window, &m_Data);
        setVSync(true);

        // ----- set GLFW callbacks -----

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height)
        {
            WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);
            data.width = width;
            data.height = height;

            WindowResizeEvent event(width, height);
            data.eventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window)
        {
            WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.eventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
        {QS_PROFILE_FUNCTION();
            WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.eventCallback(event);
                    break;
                }
                default:
                    break;
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow *window, unsigned int keycode)
        {
            WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);

            KeyTypedEvent event(keycode);
            data.eventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods)
        {
            WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.eventCallback(event);
                    break;
                }
                default:
                    break;
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset, double yOffset)
        {
            WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float) xOffset, (float) yOffset);
            data.eventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos, double yPos)
        {
            WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float) xPos, (float) yPos);
            data.eventCallback(event);
        });
    }

    void LinuxWindow::shutdown()
    {
        QS_PROFILE_FUNCTION();

        glfwDestroyWindow(m_Window);
        s_GLFWWindowCount--;

        if (s_GLFWWindowCount == 0)
        {
            glfwTerminate();
        }
    }

    void LinuxWindow::onUpdate()
    {
        QS_PROFILE_FUNCTION();

        glfwPollEvents();
        m_Context->swapBuffers();
    }

    void LinuxWindow::setVSync(bool enabled)
    {
        QS_PROFILE_FUNCTION();

        if (enabled)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }

        m_Data.vSync = enabled;
    }

    bool LinuxWindow::isVSync() const
    {
        return m_Data.vSync;
    }
}