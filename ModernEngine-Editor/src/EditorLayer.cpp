#include "mnpch.h"
#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include "ModernEngine/Scene/SceneSerializer.h"

#include "ModernEngine/Utils/PlatformUtils.h"
#include "ImGuizmo.h"

#include "ModernEngine/Math/Math.h"

namespace ModernEngine {

	extern const std::filesystem::path g_AssetsPath;

	EditorLayer::EditorLayer()
		: Layer("Editor Layer"), m_CameraController(1280.0f / 720.0f)
	{

	}

	void EditorLayer::OnAttach()
	{
		m_Checkerboard = Texture2D::Create("assets/textures/Checkerboard.png");
		m_StartButton = Texture2D::Create("Resources/Icons/Play_Button.png");
		m_StopButton = Texture2D::Create("Resources/Icons/Stop_Button.png");
		m_SimulateButton = Texture2D::Create("Resources/Icons/SimulationButton.png");

		FrameBufferSpecification fbSpec;
		fbSpec.Attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth };
		fbSpec.height = 720.0f;
		fbSpec.width = 1280.0f;
		m_FrameBuffer = FrameBuffer::Create(fbSpec);

		m_EditorScene = CreateRef<Scene>(); 
		m_ActiveScene = m_EditorScene;

		auto& commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serialize(m_ActiveScene);
			serialize.Deserialize(sceneFilePath);
		}

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.01f, 1000.0f);
		Renderer2D::SetLineWidth(4.0f);
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(DeltaTime dt)
	{
		if (ModernEngine::FrameBufferSpecification spec = m_FrameBuffer->GetSpefications();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewport(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		Renderer2D::ResetStats();
		m_FrameBuffer->Bind();

		m_EditorCamera.OnUpdate(dt);

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		m_FrameBuffer->ClearAttachments(1, -1);

		switch (m_SceneState)
		{
			case SceneState::Edit: 
			{
				m_ActiveScene->OnUpdateEditor(dt, m_EditorCamera);
				break;
			}
			case SceneState::Play:
			{
				m_ActiveScene->OnUpdateRuntime(dt);
				break;
			}
			case SceneState::Simulate:
			{
				m_ActiveScene->OnSimulationUpdate(dt, m_EditorCamera);
				break;
			}
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		auto viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = m_ViewportSize.y - my;
		int mouseX = int(mx);
		int mouseY = int(my);

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)m_ViewportSize.x && mouseY < (int)m_ViewportSize.y)
		{
			int pixelData = m_FrameBuffer->ReadPixels(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
		}

		OnOverlayRender();

		m_FrameBuffer->Unbind();
	}


	void EditorLayer::OnOverlayRender()
	{
		if (m_SceneState == SceneState::Play)
		{
			Entity camera = m_ActiveScene->GetPrimaryCamera();
			Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().m_Camera, camera.GetComponent<TransformComponent>().GetTransform());
		}
		else
		{
			Renderer2D::BeginScene(m_EditorCamera);
		}

		if (m_ShowPhysicsColliders)
		{
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view)
				{
					auto [transformC, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);
					glm::vec3 translation = transformC.Translation + glm::vec3(bc2d.offset.x, bc2d.offset.y, 0.001f);
					glm::vec3 scale = transformC.Scale * glm::vec3(bc2d.size * 2.0f, 1.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation) * glm::rotate(glm::mat4(1.0f), transformC.Rotation.z, glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1.0f), scale);
					Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));
				}
			}

			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view)
				{
					auto [transformC, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);
					glm::vec3 translation = transformC.Translation + glm::vec3(cc2d.offset.x, cc2d.offset.y, 0.001f);
					glm::vec3 scale = transformC.Scale * cc2d.Radius * 2.0f;
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation) * glm::scale(glm::mat4(1.0f), scale);
					Renderer2D::DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.05f);
				}
			}
		}

		if (m_SceneHierarchyPanel.GetSelectedEntity())
		{
			Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
			const TransformComponent& transform = selectedEntity.GetComponent<TransformComponent>();
			Renderer2D::DrawRect(transform.GetTransform(), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
		}

		Renderer2D::EndScene();
	}

	void EditorLayer::OnImGuiRender()
	{
		static bool dockspaceopen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		
		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceopen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.

				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Save...", "Ctrl+S"))
					SaveScene();

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();

		/*ImGui::Begin("Stats");

		std::string entityName = "None";
		if (m_HoveredEntity)
			entityName = m_HoveredEntity.GetComponent<TagComponent>().Tag;
		
		ImGui::Text("Entity Name: %s", entityName.c_str());
		ImGui::Text("Draw Calls: %d", Renderer2D::GetStats().DrawCalls);
		ImGui::Text("Quad Count: %d", Renderer2D::GetStats().QuadCount);
		ImGui::Text("Vertex Count: %d", Renderer2D::GetStats().GetTotalVertexCount());
		ImGui::Text("Index Count: %d", Renderer2D::GetStats().GetTotalIndexCount());

		ImGui::End();*/

		ImGui::Begin("Settings");
		ImGui::Checkbox("Show Physics Colliders", &m_ShowPhysicsColliders);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		auto viewportOffset = ImGui::GetCursorPos();
		
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvent(!m_ViewportHovered && !m_ViewportFocused);

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		
		m_ViewportSize = { viewportSize.x, viewportSize.y };

		// Rendering
		ImGui::Image((void*)m_FrameBuffer->GetColorAttachmentRendererID(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content Browser Item"))
			{
				const wchar_t* data = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(g_AssetsPath /data));
			}
			ImGui::EndDragDropTarget();
		}

		auto windowSize = ImGui::GetWindowSize();
		ImVec2 windowMinBound = ImGui::GetWindowPos();
		windowMinBound.x += viewportOffset.x;
		windowMinBound.y += viewportOffset.y;
		
		ImVec2 windowMaxBound = {windowMinBound.x + windowSize.x, windowMinBound.y + windowSize.y };
		m_ViewportBounds[0] = { windowMinBound.x, windowMinBound.y };
		m_ViewportBounds[1] = { windowMaxBound.x, windowMaxBound.y };

		// Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Camera - View and Projection 
			const glm::mat4& CameraProjection = m_EditorCamera.GetProjectionMatrix();
			glm::mat4 CameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform 
			auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 Transform = transformComponent.GetTransform();

			bool snap = Input::IsKeyPressed(MN_KEY_LEFT_CONTROL);
			float snapValue = 0.5f;
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(CameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(Transform), nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(Transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - transformComponent.Rotation; // To avoid the gimbal lock.
				transformComponent.Translation = translation;
				transformComponent.Rotation += deltaRotation;
				transformComponent.Scale = scale;
			}
		}
		
		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(MN_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<MouseButtonPressedEvent>(MN_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
		if(m_SceneState == SceneState::Edit)
			m_EditorCamera.OnEvent(e);
	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		if (e.IsRepeat())
			return false;

		bool isControlPressed = Input::IsKeyPressed(MN_KEY_LEFT_CONTROL) || Input::IsKeyPressed(MN_KEY_RIGHT_CONTROL);
		bool isShiftPressed = Input::IsKeyPressed(MN_KEY_LEFT_SHIFT) || Input::IsKeyPressed(MN_KEY_RIGHT_SHIFT);

		// Gizmos Shortcut
		switch (e.GetKeyCode())
		{
		case MN_KEY_N:
		{
			if (isControlPressed)
				NewScene();

			break;
		}

		case MN_KEY_O:
		{
			if (isControlPressed)
				OpenScene();

			break;
		}

		case MN_KEY_S:
		{
			if (isControlPressed)
			{
				if (isShiftPressed)
					SaveSceneAs();
				else
					SaveScene();

				break;
			}
		}

		case MN_KEY_Q:
			m_GizmoType = -1;
			break;

		case MN_KEY_W:
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;

		case MN_KEY_E:
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;

		case MN_KEY_R:
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}

		if (Input::IsKeyPressed(MN_KEY_D))
			OnDuplicateSelectedEntity();

		return true;
		}

		bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent & e)
		{
			if (e.GetMouseButton() == MN_MOUSE_BUTTON_LEFT && m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(MN_KEY_LEFT_CONTROL))
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);

			return false;
		}

		void EditorLayer::OnDuplicateSelectedEntity()
		{
			if (m_SceneState != SceneState::Edit)
				return;

			Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
			if (selectedEntity)
				m_EditorScene->DuplicateEntity(selectedEntity);
		}

		void EditorLayer::NewScene()
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			m_EditorPath = std::filesystem::path();
		}

		void EditorLayer::OpenScene()
		{
			std::string filepath = FileDialogs::OpenFile("ModernEngine Scene (*.modernengine)\0*.modernengine\0");
			if (!filepath.empty())
				OpenScene(filepath);
		}

		void EditorLayer::OpenScene(const std::filesystem::path & filepath)
		{
			if (m_SceneState != SceneState::Edit)
				SceneStop();

			m_EditorScene = CreateRef<Scene>();
			m_ActiveScene = m_EditorScene;
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializer Serializer(m_ActiveScene);
			Serializer.Deserialize(filepath.string());

			m_EditorPath = filepath;
		}

		void EditorLayer::SaveScene()
		{
			if (!m_EditorPath.empty())
				SerializeScene(m_ActiveScene, m_EditorPath);
			else
				SaveSceneAs();
		}

		void EditorLayer::SaveSceneAs()
		{
			std::string filepath = FileDialogs::SaveFile("ModernEngine Scene (*.modernengine)\0*.modernengine\0");
			if (!filepath.empty())
			{
				SerializeScene(m_ActiveScene, filepath);
				m_EditorPath = filepath;
			}
		}

		void EditorLayer::SerializeScene(Ref<Scene> scene, std::filesystem::path filepath)
		{
			SceneSerializer Serializer(scene);
			Serializer.Serialize(filepath.string());
		}

		void EditorLayer::UI_Toolbar()
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0 ,2 });
			ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2{ 0 , 0 });
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0, 0, 0, 0 });
			auto& colors = ImGui::GetStyle().Colors;
			const auto& hoveredButton = colors[ImGuiCol_ButtonHovered];
			const auto& activeButton = colors[ImGuiCol_ButtonActive];
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ hoveredButton.x, hoveredButton.y, hoveredButton.z, 0.5f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ activeButton.x , activeButton.y, activeButton.z, 0.5f });

			ImGui::Begin("##type", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

			bool toolbarEnable = (bool)m_ActiveScene;

			ImVec4 tintColor = ImVec4(1, 1, 1, 1);
			if (!toolbarEnable)
				tintColor.w = 0.5f; 

			// Get the icon and center it in the available content
			float size = ImGui::GetWindowHeight() - 4.0f;
			Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate) ? m_StartButton : m_StopButton;
			ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

			if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), tintColor) && toolbarEnable)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
					ScenePlay();
				else if (m_SceneState == SceneState::Play)
					SceneStop();
			}

			ImGui::SameLine();

			Ref<Texture2D> icon2 = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play) ? m_SimulateButton : m_StopButton;

			if (ImGui::ImageButton((ImTextureID)icon2->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), tintColor) && toolbarEnable)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
					SimulatePlay();
				else if (m_SceneState == SceneState::Simulate)
					SceneStop();
			}

			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(3);
			ImGui::End();
		}

		void EditorLayer::ScenePlay()
		{
			if (m_SceneState == SceneState::Simulate)
				SceneStop();

			m_SceneState = SceneState::Play;
			m_ActiveScene = Scene::Copy(m_EditorScene);
			m_ActiveScene->OnStartRuntime();

			m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		}

		void EditorLayer::SimulatePlay()
		{
			if (m_SceneState == SceneState::Play)
				SceneStop();

			m_SceneState = SceneState::Simulate;
			m_ActiveScene = Scene::Copy(m_EditorScene);
			m_ActiveScene->OnSimulationStart();

			m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		}

		void EditorLayer::SceneStop()
		{
			m_SceneState = SceneState::Edit;
			m_ActiveScene = m_EditorScene;
			m_ActiveScene->OnStopRuntime();

			m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		}

}