#pragma once

#include "Quasar/Renderer/OrthographicCamera.hpp"

#include <glm/glm.hpp>

namespace Quasar
{
    
    class Renderer2D
    {
    public:
        static void init();
        static void shutdown();

        static void beginScene(const OrthographicCamera &camera);
        static void endScene();

        static void drawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
        static void drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);
    };

} // namespace Quasar
