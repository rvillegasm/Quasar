#pragma once

#include "Quasar/Core/Base.hpp"
#include "Quasar/Events/Event.hpp"

#include <string>
#include <functional>
#include <cstdint>

namespace Quasar
{
    struct WindowProps
    {
        std::string title;
        uint32_t width;
        uint32_t height;

        WindowProps(
            const std::string &title = "Quasar Engine",
            uint32_t width = 1280,
            uint32_t height = 720) 
            : title(title), width(width), height(height) 
        {
        }
    };

    /**
     * Interface representing a desktop based window
     */
    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;

        virtual void onUpdate() = 0;

        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;

        virtual void setEventCallback(const EventCallbackFn &callback) = 0;
        virtual void setVSync(bool enabled) = 0;
        virtual bool isVSync() const = 0;

        virtual void *getNativeWindow() const = 0;

        static Scope<Window> create(const WindowProps &props = WindowProps());
    };
}