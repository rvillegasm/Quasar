#pragma once

namespace Quasar
{
    
    class GraphicsContext
    {
    public:
        virtual void init() = 0;
        virtual void swapBuffers() = 0;
    };

} // namespace Quasar
