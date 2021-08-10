#pragma once

namespace ModernEngine
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << mouseCode;
		return os;
	}
}

#define MN_MOUSE_BUTTON_0      ::ModernEngine::Mouse::Button0
#define MN_MOUSE_BUTTON_1      ::ModernEngine::Mouse::Button1
#define MN_MOUSE_BUTTON_2      ::ModernEngine::Mouse::Button2
#define MN_MOUSE_BUTTON_3      ::ModernEngine::Mouse::Button3
#define MN_MOUSE_BUTTON_4      ::ModernEngine::Mouse::Button4
#define MN_MOUSE_BUTTON_5      ::ModernEngine::Mouse::Button5
#define MN_MOUSE_BUTTON_6      ::ModernEngine::Mouse::Button6
#define MN_MOUSE_BUTTON_7      ::ModernEngine::Mouse::Button7
#define MN_MOUSE_BUTTON_LAST   ::ModernEngine::Mouse::ButtonLast
#define MN_MOUSE_BUTTON_LEFT   ::ModernEngine::Mouse::ButtonLeft
#define MN_MOUSE_BUTTON_RIGHT  ::ModernEngine::Mouse::ButtonRight
#define MN_MOUSE_BUTTON_MIDDLE ::ModernEngine::Mouse::ButtonMiddle
