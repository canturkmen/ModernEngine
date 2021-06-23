#pragma once

#include "ModernEngine/Core/Input.h"

namespace ModernEngine {

	class WindowsInput : public Input
	{
	public:
		virtual bool IsKeyPressedImpl(int key) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXPositionImpl() override;
		virtual float GetMouseYPositionImpl() override;
	};
}

