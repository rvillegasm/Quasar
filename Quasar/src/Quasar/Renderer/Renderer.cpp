#include "Renderer.hpp"

#include "Platform/OpenGL/OpenGLShader.hpp"

namespace Quasar
{

    Renderer::SceneData *Renderer::s_SceneData = new Renderer::SceneData();
    
    void Renderer::init() 
    {
        RenderCommand::init();
    }

    void Renderer::beginScene(OrthographicCamera &camera)
    {
        s_SceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
    }

    void Renderer::endScene()
    {
    }

    void Renderer::submit(
        const Ref<Shader> &shader, 
        const Ref<VertexArray> &vertexArray,
        const glm::mat4 &transform)
    {
        shader->bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_ViewProjection", s_SceneData->viewProjectionMatrix); // in order to get rid of this, a material system is needed
        std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_Transform", transform);

        vertexArray->bind();
        RenderCommand::drawIndexed(vertexArray);
    }

} // namespace Quasar
