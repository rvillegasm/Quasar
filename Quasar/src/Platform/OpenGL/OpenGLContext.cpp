#include "OpenGLContext.hpp"

#include "Quasar/Core.hpp"
#include "Quasar/Log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Quasar
{
    
    OpenGLContext::OpenGLContext(GLFWwindow *windowHandle)
        : m_WindowHandle(windowHandle)
    {
        QS_CORE_ASSERT(windowHandle, "Window handle is null!");
    }

    void OpenGLContext::init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        QS_CORE_ASSERT(status, "Failed to initialize glad!");

        QS_CORE_INFO("OpenGL Info:");
        QS_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        QS_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        QS_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
    }

    void OpenGLContext::swapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle); 
    }

} // namespace Quasar
