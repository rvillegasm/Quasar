#include "OpenGLRendererAPI.hpp"

#include "Quasar/Core/Log.hpp"

#include "Quasar/Debug/Instrumentor.hpp"

#include <glad/glad.h>

namespace Quasar
{

    void openGLMesssageCallback(
        unsigned int source,
        unsigned int type,
        unsigned int id,
        unsigned int severity,
        int length,
        const char *message,
        const void *userParam)
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            QS_CORE_CRITICAL(message);
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            QS_CORE_ERROR(message);
            break;

        case GL_DEBUG_SEVERITY_LOW:
            QS_CORE_WARN(message);
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            QS_CORE_TRACE(message);
            break;

        default:
            QS_CORE_ASSERT(false, "Unknown OpenGL Debug Severity Level!");
            break;
        }
    }

    void OpenGLRendererAPI::init()
    {
        QS_PROFILE_FUNCTION();

#ifdef QS_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(openGLMesssageCallback, nullptr);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::setClearColor(const glm::vec4 &color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::drawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount)
    {
        uint32_t count = indexCount == 0 ? vertexArray->getIndexBuffer()->getCount() : indexCount;

        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

} // namespace Quasar
