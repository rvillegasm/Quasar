#pragma once

#include "Event.hpp"

#include <sstream>

namespace Quasar
{
    class KeyEvent : public Event
    {
    protected:
        int m_KeyCode;
        KeyEvent(int keyCode) 
            : m_KeyCode(keyCode) {}

    public:
        inline int getKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    };

    class KeyPressedEvent : public KeyEvent
    {
    private:
        int m_RepeatCount;

    public:
        KeyPressedEvent(int keyCode, int repeatCount)
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
        KeyReleasedEvent(int keyCode)
            : KeyEvent(keyCode) {}

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "KeyReleased Event: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::KeyReleased)
    };
} // namespace Quasar