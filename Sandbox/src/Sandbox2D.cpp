#include "mnpch.h"
#include "Sandbox2D.h"

#include <imgui/imgui.h>
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
}

void Sanbdox2D::OnDetach()
{

}

void Sanbdox2D::OnUpdate(ModernEngine::DeltaTime dt)
{
	m_CameraController.OnUpdate(dt);

	ModernEngine::Renderer2D::ResetStats();
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
}

void Sanbdox2D::OnEvent(ModernEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}

void Sanbdox2D::OnImGuiRender()
{
	MN_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	ImGui::Text("Draw Calls: %d", ModernEngine::Renderer2D::GetStats().DrawCalls);
	ImGui::Text("Quad Count: %d", ModernEngine::Renderer2D::GetStats().QuadCount);
	ImGui::Text("Vertex Count: %d", ModernEngine::Renderer2D::GetStats().GetTotalVertexCount());
	ImGui::Text("Index Count: %d", ModernEngine::Renderer2D::GetStats().GetTotalIndexCount());
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::Image((void*)m_Checkerboard->GetRendererID(), ImVec2(1280.0f, 720.0f));
	ImGui::End();
}