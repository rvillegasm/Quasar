#pragma once

#include "Quasar/Core/Base.hpp"
#include "Quasar/Renderer/Texture.hpp"
#include "Quasar/Renderer/SubTexture2D.hpp"
#include "Quasar/Renderer/Camera.hpp"
#include "Quasar/Renderer/EditorCamera.hpp"
#include "Quasar/Renderer/OrthographicCamera.hpp"

#include <glm/glm.hpp>

namespace Quasar
{
    
    class Renderer2D
    {
    public:
        static void init();
        static void shutdown();

        static void beginScene(const Camera &camera, const glm::mat4 &transform);
        static void beginScene(const EditorCamera &camera);
        static void beginScene(const OrthographicCamera &camera); // TODO: Remove later
        static void endScene();
        static void flush();

        static void drawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
        static void drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);

        static void drawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = glm::vec4(1.0f));
        static void drawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = glm::vec4(1.0f));

        static void drawQuad(const glm::mat4 &transform, const glm::vec4 &color);
        static void drawQuad(const glm::mat4 &transform, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = glm::vec4(1.0f));

        static void drawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<SubTexture2D> &subtexture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = glm::vec4(1.0f));
        static void drawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<SubTexture2D> &subtexture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = glm::vec4(1.0f));

        // rotation is in radians
        static void drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color);
        static void drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color);

        static void drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = glm::vec4(1.0f));
        static void drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = glm::vec4(1.0f));

        static void drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const Ref<SubTexture2D> &subtexture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = glm::vec4(1.0f));
        static void drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const Ref<SubTexture2D> &subtexture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = glm::vec4(1.0f));

        struct Statistics
        {
            uint32_t drawCalls = 0;
            uint32_t quadCount = 0;

            uint32_t getTotalVertexCount() const { return quadCount * 4; }
            uint32_t getTotalIndexCount() const { return quadCount * 6; }
        };
        static void resetStats();
        static Statistics getStats();

    private:
        static void startBatch();
        static void nextBatch();
    };

} // namespace Quasar
