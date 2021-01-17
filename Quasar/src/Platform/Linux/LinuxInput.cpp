#include "Quasar/Core/Input.hpp"

#include "Quasar/Core/Application.hpp"

#include <GLFW/glfw3.h>

namespace Quasar
{
       
    bool Input::isKeyPressed(KeyCode key)
    {
        auto window = static_cast<GLFWwindow*>(
            Application::get().getWindow().getNativeWindow()
        );
        int state = glfwGetKey(window, static_cast<int32_t>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::isMouseButtonPressed(MouseCode button)
    {
        auto window = static_cast<GLFWwindow*>(
            Application::get().getWindow().getNativeWindow()
        );
        int state = glfwGetMouseButton(window, static_cast<int32_t>(button));
        return state == GLFW_PRESS;
    }

    std::pair<float, float> Input::getMousePosition()
    {
        auto window = static_cast<GLFWwindow*>(
            Application::get().getWindow().getNativeWindow()
        );
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return { (float)xpos, (float)ypos };
    }

    float Input::getMouseX()
    {
        auto [x, _] = getMousePosition();
        return x;
    }

    float Input::getMouseY()
    {
        auto [_, y] = getMousePosition();
        return y;
    }

} // namespace Quasar
