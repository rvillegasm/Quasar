#include "LinuxInput.hpp"

#include "Quasar/Application.hpp"

#include <GLFW/glfw3.h>

namespace Quasar
{
    Input *Input::s_Instance = new LinuxInput();
    
    bool LinuxInput::isKeyPressedImpl(int keycode)
    {
        auto window = static_cast<GLFWwindow*>(
            Application::get().getWindow().getNativeWindow()
        );
        int state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool LinuxInput::isMouseButtonPressedImpl(int button)
    {
        auto window = static_cast<GLFWwindow*>(
            Application::get().getWindow().getNativeWindow()
        );
        int state = glfwGetMouseButton(window, button);
        return GLFW_PRESS;
    }

    std::pair<float, float> LinuxInput::getMousePositionImpl()
    {
        auto window = static_cast<GLFWwindow*>(
            Application::get().getWindow().getNativeWindow()
        );
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return { (float)xpos, (float)ypos };
    }

    float LinuxInput::getMouseXImpl()
    {
        auto [x, _] = getMousePositionImpl();
        return x;
    }

    float LinuxInput::getMouseYImpl()
    {
        auto [_, y] = getMousePositionImpl();
        return y;
    }



} // namespace Quasar
