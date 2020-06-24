#pragma once

#include <string>
#include <functional>

#include "Quasar/Events/Event.hpp"

namespace Quasar
{
    struct WindowProps
    {
        std::string title;
        unsigned int width;
        unsigned int height;

        WindowProps(
            const std::string &title = "Quasar Engine",
            unsigned int width = 1280,
            unsigned int height = 720
        ) : title(title), width(width), height(height) 
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

        // virtual ~Window();

        virtual void onUpdate() = 0;

        virtual unsigned int getWidth() const = 0;
        virtual unsigned int getHeight() const = 0;

        virtual void setEventCallback(const EventCallbackFn &callback) = 0;
        virtual void setVSync(bool enabled) = 0;
        virtual bool isVSync() const = 0;

        static Window *create(const WindowProps &props = WindowProps());
    };
}