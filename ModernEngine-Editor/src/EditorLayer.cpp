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

	EditorLayer::EditorLayer()
		: Layer("Editor Layer"), m_CameraController(1280.0f / 720.0f)
	{

	}

	void EditorLayer::OnAttach()
	{
		m_Checkerboard = Texture2D::Create("assets/textures/Checkerboard.png");

		FrameBufferSpecification fbSpec;
		fbSpec.height = 720.0f;
		fbSpec.width = 1280.0f;
		m_FrameBuffer = FrameBuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();
		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.01f, 1000.0f);
#if 0
		Entity square = m_ActiveScene->CreateEntity("Square Entity");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 1.0f, 1.0f });
		m_Entity = square;

		Entity second_square = m_ActiveScene->CreateEntity("Second Square Entity");
		second_square.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 1.0f, 0.0f, 1.0f });

		m_Camera = m_ActiveScene->CreateEntity("Camera Entity");
		m_Camera.AddComponent<CameraComponent>();

		m_SecondCamera = m_ActiveScene->CreateEntity("Second Camera Entity");
		m_SecondCamera.AddComponent<CameraComponent>();
		m_SecondCamera.GetComponent<CameraComponent>().Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			virtual void OnCreate() override
			{
				auto& transform = GetComponent<TransformComponent>();
				transform.Translation.x = rand() % 10 - 5.0f; 
			}

		   virtual void OnDestroy() override
			{

			}

			virtual void OnUpdate(DeltaTime dt) override
			{
				auto& transform = GetComponent<TransformComponent>();
				float Speed = 5.0f;

				if (Input::IsKeyPressed(MN_KEY_A))
					transform.Translation.x -= Speed * dt;
				if (Input::IsKeyPressed(MN_KEY_D))
					transform.Translation.x += Speed * dt;
				if (Input::IsKeyPressed(MN_KEY_W))
					transform.Translation.y += Speed * dt;
				if (Input::IsKeyPressed(MN_KEY_S))
					transform.Translation.y -= Speed * dt;
			}
		};

		m_Camera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

		m_SceneHieararchyPanel.SetContext(m_ActiveScene);
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

		m_ActiveScene->OnUpdateEditor(dt, m_EditorCamera);

		m_FrameBuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		MN_PROFILE_FUNCTION();

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

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHieararchyPanel.OnImGuiRender();

		ImGui::Begin("Stats");

		ImGui::Text("Draw Calls: %d", Renderer2D::GetStats().DrawCalls);
		ImGui::Text("Quad Count: %d", Renderer2D::GetStats().QuadCount);
		ImGui::Text("Vertex Count: %d", Renderer2D::GetStats().GetTotalVertexCount());
		ImGui::Text("Index Count: %d", Renderer2D::GetStats().GetTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvent(!m_ViewportHovered && !m_ViewportFocused);

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		
		m_ViewportSize = { viewportSize.x, viewportSize.y };

		// Rendering
		ImGui::Image((void*)m_FrameBuffer->GetColorAttachmentRendererID(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// Gizmos
		Entity selectedEntity = m_SceneHieararchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Camera - View and Projection 
			auto PrimaryCamera = m_ActiveScene->GetPrimaryCamera();
			if (PrimaryCamera)
			{
				const auto& cameraCopmonent = PrimaryCamera.GetComponent<CameraComponent>().m_Camera;
				const glm::mat4& CameraProjection = cameraCopmonent.GetProjectionMatrix();
				glm::mat4 CameraView = glm::inverse(PrimaryCamera.GetComponent<TransformComponent>().GetTransform());

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
		}

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(MN_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
		m_EditorCamera.OnEvent(e);
	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
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
				if (isControlPressed && isShiftPressed)
					SaveSceneAs();

				break;
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

		return true;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHieararchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::optional<std::string> filepath = FileDialogs::OpenFile("ModernEngine Scene (*.modernengine)\0*.modernengine\0");
		if (filepath)
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHieararchyPanel.SetContext(m_ActiveScene);

			SceneSerializer Serializer(m_ActiveScene);
			Serializer.Deserialize(*filepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::optional<std::string> filepath = FileDialogs::SaveFile("ModernEngine Scene (*.modernengine)\0*.modernengine\0");
		if (filepath)
		{
			SceneSerializer Serializer(m_ActiveScene);
			Serializer.Serialize(*filepath);
		}
	}
}