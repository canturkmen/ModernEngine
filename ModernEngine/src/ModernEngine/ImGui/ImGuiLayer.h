#pragma once

#include "ModernEngine/Core.h"
#include "ModernEngine/Layer.h"

namespace ModernEngine {

	class MN_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDeattach() override;

		void OnUpdate() override;
		void OnEvent(Event& e) override; 

	private:
		float m_Time = 0.0f;
	};
}

