#pragma once

#include "Quasar/Renderer/GraphicsContext.hpp"

struct GLFWwindow;

namespace Quasar
{
    
    class OpenGLContext : public GraphicsContext
    {
    private:
        GLFWwindow *m_WindowHandle;

    public:
        OpenGLContext(GLFWwindow *windowHandle);

        void init() override;
        void swapBuffers() override;
    };

} // namespace Quasar
