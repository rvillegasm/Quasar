#pragma once

#include "Quasar/Events/Event.hpp"
#include "Quasar/Core/MouseCodes.hpp"

#include <sstream>

namespace Quasar
{
    class MouseMovedEvent : public Event
    {
    private:
        float m_MouseX, m_MouseY;

    public:
        MouseMovedEvent(const float x, const float y)
            : m_MouseX(x), m_MouseY(y) {}

        float getMouseX() const { return m_MouseX; }
        float getMouseY() const { return m_MouseY; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseMoved Event: " << m_MouseX << ", " << m_MouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    };

    class MouseScrolledEvent : public Event
    {
    private:
        float m_XOffset, m_YOffset;

    public:
        MouseScrolledEvent(const float xOffset, const float yOffset)
            : m_XOffset(xOffset), m_YOffset(yOffset) {}

        float getXOffset() const { return m_XOffset; }
        float getYOffset() const { return m_YOffset; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolled Event: " << m_XOffset << ", " << m_YOffset;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    };

    class MouseButtonEvent : public Event
    {
    protected:
        MouseCode m_Button;

        MouseButtonEvent(const MouseCode button)
            : m_Button(button) {}

    public:
        MouseCode getMouseButton() const { return m_Button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton | EventCategoryInput)
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(const MouseCode button)
            : MouseButtonEvent(button) {}

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressed Event: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::MouseButtonPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(const MouseCode button)
            : MouseButtonEvent(button) {}

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleased Event: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::MouseButtonReleased)
    };
} // namespace Quasar