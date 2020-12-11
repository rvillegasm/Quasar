#include "LinuxInput.hpp"

#include "Quasar/Core/Application.hpp"

#include <GLFW/glfw3.h>

namespace Quasar
{
    Scope<Input> Input::s_Instance = createScope<LinuxInput>();
    
    bool LinuxInput::isKeyPressedImpl(KeyCode key)
    {
        auto window = static_cast<GLFWwindow*>(
            Application::get().getWindow().getNativeWindow()
        );
        int state = glfwGetKey(window, static_cast<int32_t>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool LinuxInput::isMouseButtonPressedImpl(MouseCode button)
    {
        auto window = static_cast<GLFWwindow*>(
            Application::get().getWindow().getNativeWindow()
        );
        int state = glfwGetMouseButton(window, static_cast<int32_t>(button));
        return state == GLFW_PRESS;
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
