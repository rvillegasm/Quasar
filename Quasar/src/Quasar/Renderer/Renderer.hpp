#pragma once

#include "Quasar/Core.hpp"
#include "Quasar/Renderer/RenderCommand.hpp"
#include "Quasar/Renderer/VertexArray.hpp"
#include "Quasar/Renderer/Shader.hpp"
#include "Quasar/Renderer/OrthographicCamera.hpp"

#include <glm/glm.hpp>

#include <memory>

namespace Quasar
{

    class Renderer
    {
    private:
        struct SceneData
        {
            glm::mat4 viewProjectionMatrix;
        };

        static SceneData *s_SceneData;

    public:
        static void beginScene(OrthographicCamera &camera);
        static void endScene();

        static void submit(
            const Ref<Shader> &shader, 
            const Ref<VertexArray> &vertexArray,
            const glm::mat4 &transform = glm::mat4(1.0f)
        );

        inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
    };  

} // namespace Quasar
