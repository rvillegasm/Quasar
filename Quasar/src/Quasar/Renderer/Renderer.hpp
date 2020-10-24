#pragma once

namespace Quasar
{
    
    enum class RendererAPI
    {
        None = 0,
        OpenGL = 1,
    };

    class Renderer
    {
    private:
        static RendererAPI s_RendererAPI;

    public:
        inline static RendererAPI getAPI() { return s_RendererAPI; }
    };  

} // namespace Quasar
