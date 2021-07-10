#pragma once

#include "ModernEngine.h"

namespace ModernEngine {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(DeltaTime dt) override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

	private:
		OrthographicCameraController m_CameraController;
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.0f, 1.0f };

		Ref<Texture2D> m_Checkerboard;
		Ref<FrameBuffer> m_FrameBuffer;
	};

}
