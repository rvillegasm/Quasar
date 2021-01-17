#pragma once

#include "Quasar/Core/Base.hpp"

namespace Quasar
{
    
    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;

        virtual void init() = 0;
        virtual void swapBuffers() = 0;

        static Scope<GraphicsContext> create(void *window);
    };

} // namespace Quasar
