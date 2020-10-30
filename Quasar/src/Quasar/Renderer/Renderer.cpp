#include "Renderer.hpp"

#include "Platform/OpenGL/OpenGLShader.hpp"

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
        const std::shared_ptr<VertexArray> &vertexArray,
        const glm::mat4 &transform)
    {
        shader->bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_ViewProjection", s_SceneData->viewProjectionMatrix); // in order to get rid of this, a material system is needed
        std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_Transform", transform);

        vertexArray->bind();
        RenderCommand::drawIndexed(vertexArray);
    }

} // namespace Quasar
