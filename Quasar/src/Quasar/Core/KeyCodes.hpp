#pragma once

namespace Quasar
{
    
    typedef enum class KeyCode : uint16_t
    {
        // taken and modified from glfw3.h
        Space               = 32,
		Apostrophe          = 39, /* ' */
		Comma               = 44, /* , */
		Minus               = 45, /* - */
		Period              = 46, /* . */
		Slash               = 47, /* / */

		D0                  = 48, /* 0 */
		D1                  = 49, /* 1 */
		D2                  = 50, /* 2 */
		D3                  = 51, /* 3 */
		D4                  = 52, /* 4 */
		D5                  = 53, /* 5 */
		D6                  = 54, /* 6 */
		D7                  = 55, /* 7 */
		D8                  = 56, /* 8 */
		D9                  = 57, /* 9 */

		Semicolon           = 59, /* ; */
		Equal               = 61, /* = */

		A                   = 65,
		B                   = 66,
		C                   = 67,
		D                   = 68,
		E                   = 69,
		F                   = 70,
		G                   = 71,
		H                   = 72,
		I                   = 73,
		J                   = 74,
		K                   = 75,
		L                   = 76,
		M                   = 77,
		N                   = 78,
		O                   = 79,
		P                   = 80,
		Q                   = 81,
		R                   = 82,
		S                   = 83,
		T                   = 84,
		U                   = 85,
		V                   = 86,
		W                   = 87,
		X                   = 88,
		Y                   = 89,
		Z                   = 90,

		LeftBracket         = 91,  /* [ */
		Backslash           = 92,  /* \ */
		RightBracket        = 93,  /* ] */
		GraveAccent         = 96,  /* ` */

		World1              = 161, /* non-US #1 */
		World2              = 162, /* non-US #2 */

		/* Function keys */
		Escape              = 256,
		Enter               = 257,
		Tab                 = 258,
		Backspace           = 259,
		Insert              = 260,
		Delete              = 261,
		Right               = 262,
		Left                = 263,
		Down                = 264,
		Up                  = 265,
		PageUp              = 266,
		PageDown            = 267,
		Home                = 268,
		End                 = 269,
		CapsLock            = 280,
		ScrollLock          = 281,
		NumLock             = 282,
		PrintScreen         = 283,
		Pause               = 284,
		F1                  = 290,
		F2                  = 291,
		F3                  = 292,
		F4                  = 293,
		F5                  = 294,
		F6                  = 295,
		F7                  = 296,
		F8                  = 297,
		F9                  = 298,
		F10                 = 299,
		F11                 = 300,
		F12                 = 301,
		F13                 = 302,
		F14                 = 303,
		F15                 = 304,
		F16                 = 305,
		F17                 = 306,
		F18                 = 307,
		F19                 = 308,
		F20                 = 309,
		F21                 = 310,
		F22                 = 311,
		F23                 = 312,
		F24                 = 313,
		F25                 = 314,

		/* Keypad */
		KP0                 = 320,
		KP1                 = 321,
		KP2                 = 322,
		KP3                 = 323,
		KP4                 = 324,
		KP5                 = 325,
		KP6                 = 326,
		KP7                 = 327,
		KP8                 = 328,
		KP9                 = 329,
		KPDecimal           = 330,
		KPDivide            = 331,
		KPMultiply          = 332,
		KPSubtract          = 333,
		KPAdd               = 334,
		KPEnter             = 335,
		KPEqual             = 336,

		LeftShift           = 340,
		LeftControl         = 341,
		LeftAlt             = 342,
		LeftSuper           = 343,
		RightShift          = 344,
		RightControl        = 345,
		RightAlt            = 346,
		RightSuper          = 347,
		Menu                = 348
    } Key;

    inline std::ostream &operator<<(std::ostream &os, KeyCode keyCode)
    {
        os << static_cast<int32_t>(keyCode);
        return os;
    }

} // namespace Quasar


/* Printable keys */
#define QS_KEY_SPACE              ::Quasar::Key::Space
#define QS_KEY_APOSTROPHE         ::Quasar::Key::Apostrophe   /* ' */
#define QS_KEY_COMMA              ::Quasar::Key::Comma        /* , */
#define QS_KEY_MINUS              ::Quasar::Key::Minus        /* - */
#define QS_KEY_PERIOD             ::Quasar::Key::Period       /* . */
#define QS_KEY_SLASH              ::Quasar::Key::Slash        /* / */
#define QS_KEY_0                  ::Quasar::Key::D0
#define QS_KEY_1                  ::Quasar::Key::D1
#define QS_KEY_2                  ::Quasar::Key::D2
#define QS_KEY_3                  ::Quasar::Key::D3
#define QS_KEY_4                  ::Quasar::Key::D4
#define QS_KEY_5                  ::Quasar::Key::D5
#define QS_KEY_6                  ::Quasar::Key::D6
#define QS_KEY_7                  ::Quasar::Key::D7
#define QS_KEY_8                  ::Quasar::Key::D8
#define QS_KEY_9                  ::Quasar::Key::D9
#define QS_KEY_SEMICOLON          ::Quasar::Key::Semicolon    /* ; */
#define QS_KEY_EQUAL              ::Quasar::Key::Equal        /* = */
#define QS_KEY_A                  ::Quasar::Key::A
#define QS_KEY_B                  ::Quasar::Key::B
#define QS_KEY_C                  ::Quasar::Key::C
#define QS_KEY_D                  ::Quasar::Key::D
#define QS_KEY_E                  ::Quasar::Key::E
#define QS_KEY_F                  ::Quasar::Key::F
#define QS_KEY_G                  ::Quasar::Key::G
#define QS_KEY_H                  ::Quasar::Key::H
#define QS_KEY_I                  ::Quasar::Key::I
#define QS_KEY_J                  ::Quasar::Key::J
#define QS_KEY_K                  ::Quasar::Key::K
#define QS_KEY_L                  ::Quasar::Key::L
#define QS_KEY_M                  ::Quasar::Key::M
#define QS_KEY_N                  ::Quasar::Key::N
#define QS_KEY_O                  ::Quasar::Key::O
#define QS_KEY_P                  ::Quasar::Key::P
#define QS_KEY_Q                  ::Quasar::Key::Q
#define QS_KEY_R                  ::Quasar::Key::R
#define QS_KEY_S                  ::Quasar::Key::S
#define QS_KEY_T                  ::Quasar::Key::T
#define QS_KEY_U                  ::Quasar::Key::U
#define QS_KEY_V                  ::Quasar::Key::V
#define QS_KEY_W                  ::Quasar::Key::W
#define QS_KEY_X                  ::Quasar::Key::X
#define QS_KEY_Y                  ::Quasar::Key::Y
#define QS_KEY_Z                  ::Quasar::Key::Z
#define QS_KEY_LEFT_BRACKET       ::Quasar::Key::LeftBracket  /* [ */
#define QS_KEY_BACKSLASH          ::Quasar::Key::Backslash    /* \ */
#define QS_KEY_RIGHT_BRACKET      ::Quasar::Key::RightBracket /* ] */
#define QS_KEY_GRAVE_ACCENT       ::Quasar::Key::GraveAccent  /* ` */
#define QS_KEY_WORLD_1            ::Quasar::Key::World1       /* non-US #1 */
#define QS_KEY_WORLD_2            ::Quasar::Key::World2       /* non-US #2 */

/* Function keys */
#define QS_KEY_ESCAPE             ::Quasar::Key::Escape
#define QS_KEY_ENTER              ::Quasar::Key::Enter
#define QS_KEY_TAB                ::Quasar::Key::Tab
#define QS_KEY_BACKSPACE          ::Quasar::Key::Backspace
#define QS_KEY_INSERT             ::Quasar::Key::Insert
#define QS_KEY_DELETE             ::Quasar::Key::Delete
#define QS_KEY_RIGHT              ::Quasar::Key::Right
#define QS_KEY_LEFT               ::Quasar::Key::Left
#define QS_KEY_DOWN               ::Quasar::Key::Down
#define QS_KEY_UP                 ::Quasar::Key::Up
#define QS_KEY_PAGE_UP            ::Quasar::Key::PageUp
#define QS_KEY_PAGE_DOWN          ::Quasar::Key::PageDown
#define QS_KEY_HOME               ::Quasar::Key::Home
#define QS_KEY_END                ::Quasar::Key::End
#define QS_KEY_CAPS_LOCK          ::Quasar::Key::CapsLock
#define QS_KEY_SCROLL_LOCK        ::Quasar::Key::ScrollLock
#define QS_KEY_NUM_LOCK           ::Quasar::Key::NumLock
#define QS_KEY_PRINT_SCREEN       ::Quasar::Key::PrintScreen
#define QS_KEY_PAUSE              ::Quasar::Key::Pause
#define QS_KEY_F1                 ::Quasar::Key::F1
#define QS_KEY_F2                 ::Quasar::Key::F2
#define QS_KEY_F3                 ::Quasar::Key::F3
#define QS_KEY_F4                 ::Quasar::Key::F4
#define QS_KEY_F5                 ::Quasar::Key::F5
#define QS_KEY_F6                 ::Quasar::Key::F6
#define QS_KEY_F7                 ::Quasar::Key::F7
#define QS_KEY_F8                 ::Quasar::Key::F8
#define QS_KEY_F9                 ::Quasar::Key::F9
#define QS_KEY_F10                ::Quasar::Key::F10
#define QS_KEY_F11                ::Quasar::Key::F11
#define QS_KEY_F12                ::Quasar::Key::F12
#define QS_KEY_F13                ::Quasar::Key::F13
#define QS_KEY_F14                ::Quasar::Key::F14
#define QS_KEY_F15                ::Quasar::Key::F15
#define QS_KEY_F16                ::Quasar::Key::F16
#define QS_KEY_F17                ::Quasar::Key::F17
#define QS_KEY_F18                ::Quasar::Key::F18
#define QS_KEY_F19                ::Quasar::Key::F19
#define QS_KEY_F20                ::Quasar::Key::F20
#define QS_KEY_F21                ::Quasar::Key::F21
#define QS_KEY_F22                ::Quasar::Key::F22
#define QS_KEY_F23                ::Quasar::Key::F23
#define QS_KEY_F24                ::Quasar::Key::F24
#define QS_KEY_F25                ::Quasar::Key::F25
#define QS_KEY_KP_0               ::Quasar::Key::KP0
#define QS_KEY_KP_1               ::Quasar::Key::KP1
#define QS_KEY_KP_2               ::Quasar::Key::KP2
#define QS_KEY_KP_3               ::Quasar::Key::KP3
#define QS_KEY_KP_4               ::Quasar::Key::KP4
#define QS_KEY_KP_5               ::Quasar::Key::KP5
#define QS_KEY_KP_6               ::Quasar::Key::KP6
#define QS_KEY_KP_7               ::Quasar::Key::KP7
#define QS_KEY_KP_8               ::Quasar::Key::KP8
#define QS_KEY_KP_9               ::Quasar::Key::KP9
#define QS_KEY_KP_DECIMAL         ::Quasar::Key::KPDecimal
#define QS_KEY_KP_DIVIDE          ::Quasar::Key::KPDivide
#define QS_KEY_KP_MULTIPLY        ::Quasar::Key::KPMultiply
#define QS_KEY_KP_SUBTRACT        ::Quasar::Key::KPSubtract
#define QS_KEY_KP_ADD             ::Quasar::Key::KPAdd
#define QS_KEY_KP_ENTER           ::Quasar::Key::KPEnter
#define QS_KEY_KP_EQUAL           ::Quasar::Key::KPEqual
#define QS_KEY_LEFT_SHIFT         ::Quasar::Key::LeftShift
#define QS_KEY_LEFT_CONTROL       ::Quasar::Key::LeftControl
#define QS_KEY_LEFT_ALT           ::Quasar::Key::LeftAlt
#define QS_KEY_LEFT_SUPER         ::Quasar::Key::LeftSuper
#define QS_KEY_RIGHT_SHIFT        ::Quasar::Key::RightShift
#define QS_KEY_RIGHT_CONTROL      ::Quasar::Key::RightControl
#define QS_KEY_RIGHT_ALT          ::Quasar::Key::RightAlt
#define QS_KEY_RIGHT_SUPER        ::Quasar::Key::RightSuper
#define QS_KEY_MENU               ::Quasar::Key::Menu