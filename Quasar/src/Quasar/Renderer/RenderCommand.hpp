#pragma once

#include "Quasar/Renderer/RendererAPI.hpp"
#include "Quasar/Renderer/VertexArray.hpp"

#include <memory>

namespace Quasar
{
    
    class RenderCommand
    {
    private:
        static RendererAPI *s_RendererAPI;

    public:
        inline static void setClearColor(const glm::vec4 &color)
        {
            s_RendererAPI->setClearColor(color);
        }

        inline static void clear()
        {
            s_RendererAPI->clear();
        }

        inline static void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray)
        {
            s_RendererAPI->drawIndexed(vertexArray);
        }
    };

} // namespace Quasar
