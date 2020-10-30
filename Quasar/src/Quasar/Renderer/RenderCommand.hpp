#pragma once

#include "Quasar/Core.hpp"
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

        inline static void drawIndexed(const Ref<VertexArray> &vertexArray)
        {
            s_RendererAPI->drawIndexed(vertexArray);
        }
    };

} // namespace Quasar
