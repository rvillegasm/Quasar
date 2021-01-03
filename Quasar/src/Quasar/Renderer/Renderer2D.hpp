#pragma once

#include "Quasar/Core/Core.hpp"
#include "Quasar/Renderer/Texture.hpp"
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
        static void flush();

        static void drawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
        static void drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);
        static void drawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = glm::vec4(1.0f));
        static void drawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = glm::vec4(1.0f));

        static void drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color);
        static void drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color);
        static void drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = glm::vec4(1.0f));
        static void drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = glm::vec4(1.0f));

        struct Statistics
        {
            uint32_t drawCalls = 0;
            uint32_t quadCount = 0;

            uint32_t getTotalVertexCount() { return quadCount * 4; }
            uint32_t getTotalIndexCount() { return quadCount * 6; }
        };
        static void resetStats();
        static Statistics getStats();

    private:
        static void flushAndReset();
    };

} // namespace Quasar
