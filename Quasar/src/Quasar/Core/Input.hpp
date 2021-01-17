#pragma once

#include "Quasar/Core/Base.hpp"
#include "Quasar/Core/KeyCodes.hpp"
#include "Quasar/Core/MouseCodes.hpp"

#include <utility>

namespace Quasar
{
    
    class Input
    {
    public:
        static bool isKeyPressed(KeyCode key);

        static bool isMouseButtonPressed(MouseCode button);
        static std::pair<float, float> getMousePosition();
        static float getMouseX();
        static float getMouseY();
    };

} // namespace Quasar
