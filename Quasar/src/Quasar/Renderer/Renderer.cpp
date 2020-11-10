#include "Renderer.hpp"

#include "Quasar/Renderer/Renderer2D.hpp"

#include "Platform/OpenGL/OpenGLShader.hpp"

namespace Quasar
{

    Scope<Renderer::SceneData> Renderer::s_SceneData = createScope<Renderer::SceneData>();
    
    void Renderer::init() 
    {
        RenderCommand::init();
        Renderer2D::init();
    }
    
    void Renderer::onWindowResize(uint32_t width, uint32_t height) 
    {
        RenderCommand::setViewport(0, 0, width, height);
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
