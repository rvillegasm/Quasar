#pragma once

#include "Quasar/Events/Event.hpp"

#include <sstream>

namespace Quasar
{
    
    class WindowResizeEvent : public Event
    {
    private:
        unsigned int m_Width, m_Height;

    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
            : m_Width(width), m_Height(height) {}

        unsigned int getWidth() const { return m_Width; }
        unsigned int getHeight() const { return m_Height; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "WindowResize Event: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(EventType::WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppTickEvent : Event
    {
    public:
        AppTickEvent() = default;

        EVENT_CLASS_TYPE(EventType::AppTick)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppUpdateEvent : Event
    {
    public:
        AppUpdateEvent() = default;

        EVENT_CLASS_TYPE(EventType::AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppRenderEvent : Event
    {
    public:
        AppRenderEvent() = default;

        EVENT_CLASS_TYPE(EventType::AppRender)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

} // namespace Quasar