#pragma once

namespace ModernEngine
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define MN_KEY_SPACE           ::ModernEngine::Key::Space
#define MN_KEY_APOSTROPHE      ::ModernEngine::Key::Apostrophe    /* ' */
#define MN_KEY_COMMA           ::ModernEngine::Key::Comma         /* , */
#define MN_KEY_MINUS           ::ModernEngine::Key::Minus         /* - */
#define MN_KEY_PERIOD          ::ModernEngine::Key::Period        /* . */
#define MN_KEY_SLASH           ::ModernEngine::Key::Slash         /* / */
#define MN_KEY_0               ::ModernEngine::Key::D0
#define MN_KEY_1               ::ModernEngine::Key::D1
#define MN_KEY_2               ::ModernEngine::Key::D2
#define MN_KEY_3               ::ModernEngine::Key::D3
#define MN_KEY_4               ::ModernEngine::Key::D4
#define MN_KEY_5               ::ModernEngine::Key::D5
#define MN_KEY_6               ::ModernEngine::Key::D6
#define MN_KEY_7               ::ModernEngine::Key::D7
#define MN_KEY_8               ::ModernEngine::Key::D8
#define MN_KEY_9               ::ModernEngine::Key::D9
#define MN_KEY_SEMICOLON       ::ModernEngine::Key::Semicolon     /* ; */
#define MN_KEY_EQUAL           ::ModernEngine::Key::Equal         /* = */
#define MN_KEY_A               ::ModernEngine::Key::A
#define MN_KEY_B               ::ModernEngine::Key::B
#define MN_KEY_C               ::ModernEngine::Key::C
#define MN_KEY_D               ::ModernEngine::Key::D
#define MN_KEY_E               ::ModernEngine::Key::E
#define MN_KEY_F               ::ModernEngine::Key::F
#define MN_KEY_G               ::ModernEngine::Key::G
#define MN_KEY_H               ::ModernEngine::Key::H
#define MN_KEY_I               ::ModernEngine::Key::I
#define MN_KEY_J               ::ModernEngine::Key::J
#define MN_KEY_K               ::ModernEngine::Key::K
#define MN_KEY_L               ::ModernEngine::Key::L
#define MN_KEY_M               ::ModernEngine::Key::M
#define MN_KEY_N               ::ModernEngine::Key::N
#define MN_KEY_O               ::ModernEngine::Key::O
#define MN_KEY_P               ::ModernEngine::Key::P
#define MN_KEY_Q               ::ModernEngine::Key::Q
#define MN_KEY_R               ::ModernEngine::Key::R
#define MN_KEY_S               ::ModernEngine::Key::S
#define MN_KEY_T               ::ModernEngine::Key::T
#define MN_KEY_U               ::ModernEngine::Key::U
#define MN_KEY_V               ::ModernEngine::Key::V
#define MN_KEY_W               ::ModernEngine::Key::W
#define MN_KEY_X               ::ModernEngine::Key::X
#define MN_KEY_Y               ::ModernEngine::Key::Y
#define MN_KEY_Z               ::ModernEngine::Key::Z
#define MN_KEY_LEFT_BRACKET    ::ModernEngine::Key::LeftBracket   /* [ */
#define MN_KEY_BACKSLASH       ::ModernEngine::Key::Backslash     /* \ */
#define MN_KEY_RIGHT_BRACKET   ::ModernEngine::Key::RightBracket  /* ] */
#define MN_KEY_GRAVE_ACCENT    ::ModernEngine::Key::GraveAccent   /* ` */
#define MN_KEY_WORLD_1         ::ModernEngine::Key::World1        /* non-US #1 */
#define MN_KEY_WORLD_2         ::ModernEngine::Key::World2        /* non-US #2 */

/* Function keys */
#define MN_KEY_ESCAPE          ::ModernEngine::Key::Escape
#define MN_KEY_ENTER           ::ModernEngine::Key::Enter
#define MN_KEY_TAB             ::ModernEngine::Key::Tab
#define MN_KEY_BACKSPACE       ::ModernEngine::Key::Backspace
#define MN_KEY_INSERT          ::ModernEngine::Key::Insert
#define MN_KEY_DELETE          ::ModernEngine::Key::Delete
#define MN_KEY_RIGHT           ::ModernEngine::Key::Right
#define MN_KEY_LEFT            ::ModernEngine::Key::Left
#define MN_KEY_DOWN            ::ModernEngine::Key::Down
#define MN_KEY_UP              ::ModernEngine::Key::Up
#define MN_KEY_PAGE_UP         ::ModernEngine::Key::PageUp
#define MN_KEY_PAGE_DOWN       ::ModernEngine::Key::PageDown
#define MN_KEY_HOME            ::ModernEngine::Key::Home
#define MN_KEY_END             ::ModernEngine::Key::End
#define MN_KEY_CAPS_LOCK       ::ModernEngine::Key::CapsLock
#define MN_KEY_SCROLL_LOCK     ::ModernEngine::Key::ScrollLock
#define MN_KEY_NUM_LOCK        ::ModernEngine::Key::NumLock
#define MN_KEY_PRINT_SCREEN    ::ModernEngine::Key::PrintScreen
#define MN_KEY_PAUSE           ::ModernEngine::Key::Pause
#define MN_KEY_F1              ::ModernEngine::Key::F1
#define MN_KEY_F2              ::ModernEngine::Key::F2
#define MN_KEY_F3              ::ModernEngine::Key::F3
#define MN_KEY_F4              ::ModernEngine::Key::F4
#define MN_KEY_F5              ::ModernEngine::Key::F5
#define MN_KEY_F6              ::ModernEngine::Key::F6
#define MN_KEY_F7              ::ModernEngine::Key::F7
#define MN_KEY_F8              ::ModernEngine::Key::F8
#define MN_KEY_F9              ::ModernEngine::Key::F9
#define MN_KEY_F10             ::ModernEngine::Key::F10
#define MN_KEY_F11             ::ModernEngine::Key::F11
#define MN_KEY_F12             ::ModernEngine::Key::F12
#define MN_KEY_F13             ::ModernEngine::Key::F13
#define MN_KEY_F14             ::ModernEngine::Key::F14
#define MN_KEY_F15             ::ModernEngine::Key::F15
#define MN_KEY_F16             ::ModernEngine::Key::F16
#define MN_KEY_F17             ::ModernEngine::Key::F17
#define MN_KEY_F18             ::ModernEngine::Key::F18
#define MN_KEY_F19             ::ModernEngine::Key::F19
#define MN_KEY_F20             ::ModernEngine::Key::F20
#define MN_KEY_F21             ::ModernEngine::Key::F21
#define MN_KEY_F22             ::ModernEngine::Key::F22
#define MN_KEY_F23             ::ModernEngine::Key::F23
#define MN_KEY_F24             ::ModernEngine::Key::F24
#define MN_KEY_F25             ::ModernEngine::Key::F25

/* Keypad */
#define MN_KEY_KP_0            ::ModernEngine::Key::KP0
#define MN_KEY_KP_1            ::ModernEngine::Key::KP1
#define MN_KEY_KP_2            ::ModernEngine::Key::KP2
#define MN_KEY_KP_3            ::ModernEngine::Key::KP3
#define MN_KEY_KP_4            ::ModernEngine::Key::KP4
#define MN_KEY_KP_5            ::ModernEngine::Key::KP5
#define MN_KEY_KP_6            ::ModernEngine::Key::KP6
#define MN_KEY_KP_7            ::ModernEngine::Key::KP7
#define MN_KEY_KP_8            ::ModernEngine::Key::KP8
#define MN_KEY_KP_9            ::ModernEngine::Key::KP9
#define MN_KEY_KP_DECIMAL      ::ModernEngine::Key::KPDecimal
#define MN_KEY_KP_DIVIDE       ::ModernEngine::Key::KPDivide
#define MN_KEY_KP_MULTIPLY     ::ModernEngine::Key::KPMultiply
#define MN_KEY_KP_SUBTRACT     ::ModernEngine::Key::KPSubtract
#define MN_KEY_KP_ADD          ::ModernEngine::Key::KPAdd
#define MN_KEY_KP_ENTER        ::ModernEngine::Key::KPEnter
#define MN_KEY_KP_EQUAL        ::ModernEngine::Key::KPEqual

#define MN_KEY_LEFT_SHIFT      ::ModernEngine::Key::LeftShift
#define MN_KEY_LEFT_CONTROL    ::ModernEngine::Key::LeftControl
#define MN_KEY_LEFT_ALT        ::ModernEngine::Key::LeftAlt
#define MN_KEY_LEFT_SUPER      ::ModernEngine::Key::LeftSuper
#define MN_KEY_RIGHT_SHIFT     ::ModernEngine::Key::RightShift
#define MN_KEY_RIGHT_CONTROL   ::ModernEngine::Key::RightControl
#define MN_KEY_RIGHT_ALT       ::ModernEngine::Key::RightAlt
#define MN_KEY_RIGHT_SUPER     ::ModernEngine::Key::RightSuper
#define MN_KEY_MENU            ::ModernEngine::Key::Menu