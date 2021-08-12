#pragma once

#include "ModernEngine.h"
#include "Panels/SceneHierarchyPanel.h"
#include "ModernEngine/Renderer/EditorCamera.h"

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
		bool OnKeyPressedEvent(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();

	private:
		Ref<Scene> m_ActiveScene;

		OrthographicCameraController m_CameraController;
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.0f, 1.0f };
		Ref<Texture2D> m_Checkerboard;
		Ref<FrameBuffer> m_FrameBuffer;

		EditorCamera m_EditorCamera;
		Entity m_Entity;
		Entity m_Camera;
		Entity m_SecondCamera;
		bool m_PrimaryCamera = true;

		glm::vec2 m_ViewportBounds[2];
		glm::vec2 m_ViewportSize = { 0, 0 };
		bool m_ViewportFocused = false, m_ViewportHovered = false;


		int m_GizmoType = -1;
		SceneHierarchyPanel m_SceneHieararchyPanel;
	};

}
