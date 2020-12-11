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

        KeyEvent(KeyCode keyCode)
            : m_KeyCode(keyCode) {}

    public:
        inline KeyCode getKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    };

    class KeyPressedEvent : public KeyEvent
    {
    private:
        int m_RepeatCount;

    public:
        KeyPressedEvent(KeyCode keyCode, int repeatCount)
            : KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

        inline int getRepeatCount() const { return m_RepeatCount; }

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
        KeyReleasedEvent(KeyCode keyCode)
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
        KeyTypedEvent(KeyCode keyCode)
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