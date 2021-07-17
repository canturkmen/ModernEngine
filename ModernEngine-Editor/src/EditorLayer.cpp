#include "mnpch.h"
#include "EditorLayer.h"

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

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

		m_Camera = m_ActiveScene->CreateEntity("Camera Entity");
		m_Camera.AddComponent<CameraComponent>(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));

		m_SecondCamera = m_ActiveScene->CreateEntity("Second Camera");
		m_SecondCamera.AddComponent<CameraComponent>(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f)); 

		m_Entity = square;
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(DeltaTime dt)
	{
		Renderer2D::ResetStats();
		m_FrameBuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		m_ActiveScene->OnUpdate(dt);

		m_FrameBuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
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
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");

		ImGui::Text("Draw Calls: %d", Renderer2D::GetStats().DrawCalls);
		ImGui::Text("Quad Count: %d", Renderer2D::GetStats().QuadCount);
		ImGui::Text("Vertex Count: %d", Renderer2D::GetStats().GetTotalVertexCount());
		ImGui::Text("Index Count: %d", Renderer2D::GetStats().GetTotalIndexCount());    

		auto& tag = m_Entity.GetComponent<TagComponent>().Tag;
		ImGui::Text("Tag: %s", tag.c_str());

		auto& color = m_Entity.GetComponent<SpriteRendererComponent>().Color;
		ImGui::ColorEdit4("Color: ", glm::value_ptr(color));

		ImGui::DragFloat3("Camera Transform", glm::value_ptr(m_Camera.GetComponent<TransformComponent>().Transform[3]));

		if (ImGui::Checkbox("Main Camera", &m_PrimaryCamera))
		{
			m_Camera.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
			m_SecondCamera.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvent(!m_ViewportHovered || !m_ViewportFocused);

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *(glm::vec2*)&viewportSize && viewportSize.x > 0 && viewportSize.y > 0)
		{
			m_FrameBuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			m_ViewportSize = { viewportSize.x, viewportSize.y };
			m_CameraController.OnResize(viewportSize.x, viewportSize.y);
		}
		ImGui::Image((void*)m_FrameBuffer->GetColorAttachmentRendererID(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}
}

