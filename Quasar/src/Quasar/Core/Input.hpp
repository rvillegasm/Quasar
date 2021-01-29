#pragma once

#include "Quasar/Core/KeyCodes.hpp"
#include "Quasar/Core/MouseCodes.hpp"

#include <glm/glm.hpp>

namespace Quasar
{
    
    class Input
    {
    public:
        static bool isKeyPressed(KeyCode key);

        static bool isMouseButtonPressed(MouseCode button);
        static glm::vec2 getMousePosition();
        static float getMouseX();
        static float getMouseY();
    };

} // namespace Quasar
