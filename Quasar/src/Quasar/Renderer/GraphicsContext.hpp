#pragma once

#include "Quasar/Core/Core.hpp"

namespace Quasar
{
    
    class GraphicsContext
    {
    public:
        virtual void init() = 0;
        virtual void swapBuffers() = 0;

        static Scope<GraphicsContext> create(void *window);
    };

} // namespace Quasar
