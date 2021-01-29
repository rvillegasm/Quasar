#include "OpenGLContext.hpp"

#include "Quasar/Core/Base.hpp"
#include "Quasar/Core/Log.hpp"

#include "Quasar/Debug/Instrumentor.hpp"

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
        QS_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        QS_CORE_ASSERT(status, "Failed to initialize glad!");

        QS_CORE_INFO("OpenGL Info:");
        QS_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        QS_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        QS_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

        QS_CORE_ASSERT(
            GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5),
            "Quasar requires at least OpenGL version 4.5!"
        );
    }

    void OpenGLContext::swapBuffers()
    {
        QS_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle); 
    }

} // namespace Quasar
