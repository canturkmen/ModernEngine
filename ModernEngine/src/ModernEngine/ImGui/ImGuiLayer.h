#pragma once

#include "ModernEngine/Layer.h"

#include "ModernEngine/Events/KeyEvent.h"
#include "ModernEngine/Events/MouseEvent.h"
#include "ModernEngine/Events/ApplicationEvent.h"

namespace ModernEngine {

	class MN_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		virtual void OnImGuiRender() override;
		void End();

	private:
		float m_Time = 0.0f;
	};
}

