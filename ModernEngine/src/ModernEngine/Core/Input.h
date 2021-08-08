#pragma once

#include "Base.h"
#include "MouseButtonCodes.h"
#include "KeyCodes.h"

namespace ModernEngine {

	// This is an interface for the sub input classes such as Windows input, Linux input and MAC input
	class Input
	{
	public:
		virtual ~Input() = default;

		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseXPosition();
		static float GetMouseYPosition();
	};
}