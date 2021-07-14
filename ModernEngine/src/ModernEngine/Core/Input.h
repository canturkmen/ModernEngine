#pragma once

#include "Core.h"

namespace ModernEngine {

	// This is an interface for the sub input classes such as Windows input, Linux input and MAC input
	class Input
	{
	public:
		static bool IsKeyPressed(int key);
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseXPosition();
		static float GetMouseYPosition();
	};
}