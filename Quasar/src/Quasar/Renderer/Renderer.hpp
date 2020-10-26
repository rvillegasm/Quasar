#pragma once

#include "Quasar/Renderer/RenderCommand.hpp"
#include "Quasar/Renderer/VertexArray.hpp"

#include <memory>

namespace Quasar
{

    class Renderer
    {
    public:
        static void beginScene();
        static void endScene();

        static void submit(const std::shared_ptr<VertexArray> &vertexArray);

        inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
    };  

} // namespace Quasar
