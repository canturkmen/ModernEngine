#include "mnpch.h"
#include "Sandbox2D.h"

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sanbdox2D::Sanbdox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sanbdox2D::OnAttach()
{
	m_Checkerboard = ModernEngine::Texture2D::Create("assets/textures/Checkerboard.png");

	ModernEngine::FrameBufferSpecification fbSpec;
	fbSpec.height = 720.0f;
	fbSpec.width = 1280.0f;
	m_FrameBuffer = ModernEngine::FrameBuffer::Create(fbSpec);

	m_ParticleProps.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_ParticleProps.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_ParticleProps.SizeBegin = 0.5f;
	m_ParticleProps.SizeVariation = 0.3f;
	m_ParticleProps.SizeEnd = 0.0f;
	m_ParticleProps.LifeTime = 1.0f;
	m_ParticleProps.Velocity = { 0.0f, 0.0f };
	m_ParticleProps.VelocityVariation = { 3.0f, 1.0f };
	m_ParticleProps.Position = { 0.0f, 0.0f };

	m_CameraController.SetZoomLevel(5.0f);
}

void Sanbdox2D::OnDetach()
{

}

void Sanbdox2D::OnUpdate(ModernEngine::DeltaTime dt)
{
	m_CameraController.OnUpdate(dt);

	ModernEngine::Renderer2D::ResetStats();
	m_FrameBuffer->Bind();
	ModernEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	ModernEngine::RenderCommand::Clear();

	static float Rotation = 0.0f;
	Rotation += dt * 50;

	ModernEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	ModernEngine::Renderer2D::DrawQuad({ -1.0f, 1.0f }, { 1.0f, 1.0f }, m_SquareColor);
	ModernEngine::Renderer2D::DrawQuad({ 1.5f, -1.5f }, { 1.0f, 1.0f }, m_SquareColor);
	ModernEngine::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_Checkerboard, 10.0f);
	ModernEngine::Renderer2D::DrawRotateQuad({ -2.0f, -2.0f }, { 1.5f, 1.5f }, Rotation, m_Checkerboard, 10.0f);
	ModernEngine::Renderer2D::EndScene();

	ModernEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	for (float y = -5.0f; y < 5.0f; y += 0.5f)
	{
		for (float x = -5.0f; x < 5.0f; x += 0.5f)
		{
			glm::vec4 color = { (x + 5.0f) / 10.0f, 0.3f, (y + 5.0f) / 10.0f, 0.7f };
			ModernEngine::Renderer2D::DrawQuad({ x, y }, { 0.5f, 0.5f }, color);
		}
	}

	ModernEngine::Renderer2D::EndScene();
	m_FrameBuffer->Unbind();

#if 0
	if (ModernEngine::Input::IsMouseButtonPressed(MN_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = ModernEngine::Input::GetMousePosition();
		float width = ModernEngine::Application::Get().GetWindow().GetWidth();
		float height = ModernEngine::Application::Get().GetWindow().GetHeight();

		ModernEngine::OrthographicCameraBounds bounds = m_CameraController.GetBounds();
		glm::vec3 position = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_ParticleProps.Position = { x + position.x, y + position.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_ParticleProps);
	}

	m_ParticleSystem.OnUpdate(dt);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());
#endif
}

void Sanbdox2D::OnEvent(ModernEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}

void Sanbdox2D::OnImGuiRender()
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
			if(ImGui::MenuItem("Exit")) ModernEngine::Application::Get().Close();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Begin("Settings");

	ImGui::Text("Draw Calls: %d", ModernEngine::Renderer2D::GetStats().DrawCalls);
	ImGui::Text("Quad Count: %d", ModernEngine::Renderer2D::GetStats().QuadCount);
	ImGui::Text("Vertex Count: %d", ModernEngine::Renderer2D::GetStats().GetTotalVertexCount());
	ImGui::Text("Index Count: %d", ModernEngine::Renderer2D::GetStats().GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::Image((void*)m_FrameBuffer->GetColorAttachmentRendererID(), ImVec2(1280.0f, 720.0f));
	ImGui::End();

	ImGui::End();
}