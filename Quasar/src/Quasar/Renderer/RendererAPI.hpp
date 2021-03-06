#pragma once

#include "Quasar/Core/Base.hpp"
#include "Quasar/Renderer/VertexArray.hpp"

#include <glm/glm.hpp>

#include <cstdint>

namespace Quasar
{
    
    class RendererAPI
    {
    public:
        enum class API
        {
            None = 0,
            OpenGL = 1,
        };

    private:
        static API s_API;

    public:
        virtual ~RendererAPI() = default;

        virtual void init() = 0;

        virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

        virtual void setClearColor(const glm::vec4 &color) = 0;
        virtual void clear() = 0;

        virtual void drawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount = 0) = 0;

        static API getAPI() { return s_API; }
        static Scope<RendererAPI> create();
    };

} // namespace Quasar
