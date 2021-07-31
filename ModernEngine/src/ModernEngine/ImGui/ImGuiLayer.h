#pragma once

#include "ModernEngine/Core/Layer.h"

#include "ModernEngine/Events/KeyEvent.h"
#include "ModernEngine/Events/MouseEvent.h"
#include "ModernEngine/Events/ApplicationEvent.h"

namespace ModernEngine {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvent(bool eventblocked) { m_EventBlocked = eventblocked; }

	private:
		void SetTheme();

	private:
		bool m_EventBlocked = true;
		float m_Time = 0.0f;
	};
}

