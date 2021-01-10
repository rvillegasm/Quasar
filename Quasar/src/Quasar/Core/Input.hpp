#pragma once

#include "Quasar/Core/Base.hpp"
#include "Quasar/Core/KeyCodes.hpp"
#include "Quasar/Core/MouseCodes.hpp"

#include <utility>

namespace Quasar
{
    
    class Input
    {
    private:
        static Scope<Input> s_Instance;

    protected:
        Input() = default;

        virtual bool isKeyPressedImpl(KeyCode key) = 0;
        virtual bool isMouseButtonPressedImpl(MouseCode button) = 0;
        virtual std::pair<float, float> getMousePositionImpl() = 0;
        virtual float getMouseXImpl() = 0;
        virtual float getMouseYImpl() = 0;

    public:
        Input(const Input&) = delete;
        Input &operator=(const Input&) = delete;

        static bool isKeyPressed(KeyCode key) { return s_Instance->isKeyPressedImpl(key); }

        static bool isMouseButtonPressed(MouseCode button) { return s_Instance->isMouseButtonPressedImpl(button); }
        static std::pair<float, float> getMousePosition() { return s_Instance->getMousePositionImpl(); }
        static float getMouseX() { return s_Instance->getMouseXImpl(); }
        static float getMouseY() { return s_Instance->getMouseYImpl(); }

        static Scope<Input> create();
    };

} // namespace Quasar
