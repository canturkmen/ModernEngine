#pragma once

#include "ModernEngine.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
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
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void OnDuplicateSelectedEntity();

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& filepath);
		void SaveScene();
		void SaveSceneAs();
		void SerializeScene(Ref<Scene> scene, std::filesystem::path filepath);

		void UI_Toolbar();

		void ScenePlay();
		void SimulatePlay();
		void SceneStop();
		void ScenePause();


		void OnOverlayRender();

	private:
		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;

		OrthographicCameraController m_CameraController;
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.0f, 1.0f };
		Ref<Texture2D> m_Checkerboard;
		Ref<FrameBuffer> m_FrameBuffer;

		EditorCamera m_EditorCamera;
		Entity m_HoveredEntity;
		bool m_PrimaryCamera = true;
		std::filesystem::path m_EditorPath;

		glm::vec2 m_ViewportBounds[2];
		glm::vec2 m_ViewportSize = { 0, 0 };
		bool m_ViewportFocused = false, m_ViewportHovered = false;

		int m_GizmoType = -1;
		bool m_ShowPhysicsColliders = false;

		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		Ref<Texture2D> m_StartButton, m_StopButton, m_SimulateButton, m_PauseButton, m_StepForwardButton;
		enum class SceneState 
		{
			Edit = 0, Play = 1, Simulate = 2	
		};

		SceneState m_SceneState = SceneState::Edit;
	};   
}
