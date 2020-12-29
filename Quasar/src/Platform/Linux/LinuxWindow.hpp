#pragma once

#include "Quasar/Core/Window.hpp"
#include "Quasar/Renderer/GraphicsContext.hpp"

#include "GLFW/glfw3.h"

#include <string>

namespace Quasar
{
    class LinuxWindow final : public Window
    {
    private:
        GLFWwindow *m_Window;
        Scope<GraphicsContext> m_Context;

        struct WindowData
        {
            std::string title;
            unsigned int width, height;
            bool vSync;

            EventCallbackFn eventCallback;
        } m_Data;

        virtual void init(const WindowProps &props);
        virtual void shutdown();

    public:
        LinuxWindow(const WindowProps &props);
        virtual ~LinuxWindow();

        void onUpdate() override;

        unsigned int getWidth() const override { return m_Data.width; }
        unsigned int getHeight() const override { return m_Data.height; }

        void setEventCallback(const EventCallbackFn &callback) override { m_Data.eventCallback = callback; }
        void setVSync(bool enabled) override;
        bool isVSync() const override;

        virtual void *getNativeWindow() const override { return m_Window; }
    };
}