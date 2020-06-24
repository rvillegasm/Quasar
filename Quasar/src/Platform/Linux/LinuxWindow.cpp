#include "LinuxWindow.hpp"

#include "Quasar/Core.hpp"
#include "Quasar/Log.hpp"

namespace Quasar
{
    static bool s_GLFWInitialized = false;

    Window *Window::create(const WindowProps &props)
    {
        return new LinuxWindow(props);
    }

    LinuxWindow::LinuxWindow(const WindowProps &props)
    {
        init(props);
    }

    LinuxWindow::~LinuxWindow()
    {
        shutdown();
    }

    void LinuxWindow::init(const WindowProps &props)
    {
        m_Data.title = props.title;
        m_Data.width = props.width;
        m_Data.height = props.height;

        QS_CORE_INFO(
            "Creating window {0} ({1}, {2})",
            props.title, props.width, props.height
        );

        if (!s_GLFWInitialized)
        {
            int success = glfwInit();
            QS_CORE_ASSERT(success, "Could not initialize GLFW!");
            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow(
            (int) props.width,
            (int) props.height,
            m_Data.title.c_str(),
            nullptr,
            nullptr
        );
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        setVSync(true);
    }

    void LinuxWindow::shutdown()
    {
        glfwDestroyWindow(m_Window);
    }

    void LinuxWindow::onUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window); 
    }

    void LinuxWindow::setVSync(bool enabled)
    {
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