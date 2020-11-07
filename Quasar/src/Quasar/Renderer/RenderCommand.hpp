#pragma once

#include "Quasar/Core/Core.hpp"
#include "Quasar/Renderer/RendererAPI.hpp"
#include "Quasar/Renderer/VertexArray.hpp"

#include <glm/glm.hpp>

#include <cstdint>

namespace Quasar
{
    
    class RenderCommand
    {
    private:
        static Scope<RendererAPI> s_RendererAPI;

    public:
        inline static void init()
        {
            s_RendererAPI->init();
        }

        inline static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            s_RendererAPI->setViewport(x, y, width, height);
        }

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
