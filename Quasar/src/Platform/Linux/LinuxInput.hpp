#pragma once

#include "Quasar/Core/Input.hpp"

#include <utility>

namespace Quasar
{
    
    class LinuxInput final : public Input
    {
    protected:
        bool isKeyPressedImpl(KeyCode key) override;
        bool isMouseButtonPressedImpl(MouseCode button) override;
        std::pair<float, float> getMousePositionImpl() override;
        float getMouseXImpl() override;
        float getMouseYImpl() override;
    };

} // namespace Quasar