#include "OpenGLContext.hpp"

#include "Quasar/Core.hpp"

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
    }

    void OpenGLContext::swapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle); 
    }

} // namespace Quasar
