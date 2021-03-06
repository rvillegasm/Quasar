#pragma once

#include "Quasar/Core/Base.hpp"
#include "Quasar/Renderer/RendererAPI.hpp"
#include "Quasar/Renderer/VertexArray.hpp"

namespace Quasar
{
    
    class OpenGLRendererAPI final : public RendererAPI
    {
    private:

    public:
        void init() override;

        void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

        void setClearColor(const glm::vec4 &color) override;
        void clear() override;

        void drawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount = 0) override;
    };

} // namespace Quasar
