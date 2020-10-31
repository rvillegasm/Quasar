#pragma once

#include "Quasar/Core.hpp"
#include "Quasar/Renderer/RendererAPI.hpp"
#include "Quasar/Renderer/VertexArray.hpp"

namespace Quasar
{
    
    class OpenGLRendererAPI final : public RendererAPI
    {
    private:

    public:
        void init() override;

        void setClearColor(const glm::vec4 &color) override;
        void clear() override;

        void drawIndexed(const Ref<VertexArray> &vertexArray) override;
    };

} // namespace Quasar
