#include "Renderer.hpp"

namespace Quasar
{
    Renderer::SceneData *Renderer::s_SceneData = new Renderer::SceneData();
    
    void Renderer::beginScene(OrthographicCamera &camera)
    {
        s_SceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
    }

    void Renderer::endScene()
    {
    }

    void Renderer::submit(
        const std::shared_ptr<Shader> &shader, 
        const std::shared_ptr<VertexArray> &vertexArray
    )
    {
        shader->bind();
        shader->uploadUniformMat4("u_ViewProjection", s_SceneData->viewProjectionMatrix);

        vertexArray->bind();
        RenderCommand::drawIndexed(vertexArray);
    }

} // namespace Quasar
