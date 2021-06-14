#pragma once

#include "Core.h"

namespace ModernEngine {

	// This is an interface for the sub input classes such as Windows input, Linux input and MAC input
	class MN_API Input
	{
	public:
		static bool IsKeyPressed(int key) { return s_InputInstance->IsKeyPressedImpl(key); }
		static bool IsMouseButtonPressed(int button) { return s_InputInstance->IsMouseButtonPressedImpl(button); }
		std::pair<float, float> GetMousePosition() { return s_InputInstance->GetMousePositionImpl(); }
		float GetMouseXPosition() { return s_InputInstance->GetMouseXPositionImpl(); }
		float GetMouseYPosition() { return s_InputInstance->GetMouseYPositionImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int key) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXPositionImpl() = 0;
		virtual float GetMouseYPositionImpl() = 0;

	private:
		static Input* s_InputInstance;
	};
}