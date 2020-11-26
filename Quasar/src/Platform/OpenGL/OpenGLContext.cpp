#include "OpenGLContext.hpp"

#include "Quasar/Core/Core.hpp"
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

        #ifdef QS_ENABLE_ASSERTS
            int versionMajor;
            int versionMinor;
            glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
            glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

            QS_CORE_ASSERT(
                versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5),
                "Quasar requires at least OpenGL version 4.5!"
            );
        #endif
    }

    void OpenGLContext::swapBuffers()
    {
        QS_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle); 
    }

} // namespace Quasar
