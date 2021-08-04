#include "mnpch.h"
#include "EditorLayer.h"

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include "ModernEngine/Scene/SceneSerializer.h"

#include "ModernEngine/Utils/PlatformUtils.h"

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

		m_ActiveScene = std::make_shared<Scene>();

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
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		Renderer2D::ResetStats();
		m_FrameBuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		m_ActiveScene->OnUpdate(dt);

		m_FrameBuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& e)
	{

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
				{
					m_ActiveScene = std::make_shared<Scene>();
					m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
					m_SceneHieararchyPanel.SetContext(m_ActiveScene);
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					std::string filepath = FileDialogs::OpenFile("Modern Engine (*.modernengine)\0*.modernengine\0");
					if (!filepath.empty())
					{
						m_ActiveScene = std::make_shared<Scene>();
						m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
						m_SceneHieararchyPanel.SetContext(m_ActiveScene);

						SceneSerializer Serializer(m_ActiveScene);
						Serializer.Deserialize(filepath);
					}
				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					std::string filepath = FileDialogs::SaveFile("Modern Engine (*.modernengine)\0*.modernengine\0");
					if (!filepath.empty())
					{
						SceneSerializer Serializer(m_ActiveScene);
						Serializer.Serialize(filepath);
					}
				}

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
		Application::Get().GetImGuiLayer()->BlockEvent(!m_ViewportHovered || !m_ViewportFocused);

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		
		m_ViewportSize = { viewportSize.x, viewportSize.y };

		ImGui::Image((void*)m_FrameBuffer->GetColorAttachmentRendererID(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}
}

