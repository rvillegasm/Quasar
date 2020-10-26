#pragma once

#include "Quasar/Renderer/VertexArray.hpp"

#include <glm/glm.hpp>

#include <memory>

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
        virtual void setClearColor(const glm::vec4 &color) = 0;
        virtual void clear() = 0;

        virtual void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray) = 0;

        inline static API getAPI() { return s_API; }
    };

} // namespace Quasar
