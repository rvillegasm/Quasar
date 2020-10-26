#pragma once

#include "Quasar/Renderer/RendererAPI.hpp"
#include "Quasar/Renderer/VertexArray.hpp"

namespace Quasar
{
    
    class OpenGLRendererAPI final : public RendererAPI
    {
    private:

    public:
        void setClearColor(const glm::vec4 &color) override;
        void clear() override;

        void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray) override;

    };

} // namespace Quasar
