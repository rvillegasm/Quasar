#pragma once

#include "Quasar/Events/Event.hpp"
#include "Quasar/Core/KeyCodes.hpp"

#include <sstream>

namespace Quasar
{
    
    class KeyEvent : public Event
    {
    protected:
        KeyCode m_KeyCode;

        KeyEvent(const KeyCode keyCode)
            : m_KeyCode(keyCode) {}

    public:
        KeyCode getKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    };

    class KeyPressedEvent : public KeyEvent
    {
    private:
        uint16_t m_RepeatCount;

    public:
        KeyPressedEvent(const KeyCode keyCode, const uint16_t repeatCount)
            : KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

        uint16_t getRepeatCount() const { return m_RepeatCount; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "KeyPressed Event: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::KeyPressed)
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(const KeyCode keyCode)
            : KeyEvent(keyCode) {}

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "KeyReleased Event: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::KeyReleased)
    };

    class KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(const KeyCode keyCode)
            : KeyEvent(keyCode) {}

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "KeyTyped Event: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::KeyTyped)
    };

} // namespace Quasar