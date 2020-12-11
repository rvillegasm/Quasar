#pragma once

namespace Quasar
{
    
    typedef enum class MouseCode : uint16_t
    {
        // taken and modified from glfw3.h
        Button0      = 0,
        Button1      = 1,
        Button2      = 2,
        Button3      = 3,
        Button4      = 4,
        Button5      = 5,
        Button6      = 6,
        Button7      = 7,

        ButtonLast   = Button7,
        ButtonLeft   = Button0,
        ButtonRight  = Button1,
        ButtonMiddle = Button2
    } Mouse;

    inline std::ostream &operator<<(std::ostream &os, MouseCode mouseCode)
    {
        os << static_cast<int32_t>(mouseCode);
        return os;
    }

} // namespace Quasar

#define QS_MOUSE_BUTTON_0         ::Quasar::Mouse::Button0
#define QS_MOUSE_BUTTON_1         ::Quasar::Mouse::Button1
#define QS_MOUSE_BUTTON_2         ::Quasar::Mouse::Button2
#define QS_MOUSE_BUTTON_3         ::Quasar::Mouse::Button3
#define QS_MOUSE_BUTTON_4         ::Quasar::Mouse::Button4
#define QS_MOUSE_BUTTON_5         ::Quasar::Mouse::Button5
#define QS_MOUSE_BUTTON_6         ::Quasar::Mouse::Button6
#define QS_MOUSE_BUTTON_7         ::Quasar::Mouse::Button7
#define QS_MOUSE_BUTTON_LAST      ::Quasar::Mouse::ButtonLast
#define QS_MOUSE_BUTTON_LEFT      ::Quasar::Mouse::ButtonLeft
#define QS_MOUSE_BUTTON_RIGHT     ::Quasar::Mouse::ButtonRight
#define QS_MOUSE_BUTTON_MIDDLE    ::Quasar::Mouse::ButtonMiddle