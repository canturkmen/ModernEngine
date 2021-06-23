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

}

void Sanbdox2D::OnDetach()
{

}

void Sanbdox2D::OnUpdate(ModernEngine::DeltaTime dt)
{
	m_CameraController.OnUpdate(dt);

	ModernEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	ModernEngine::RenderCommand::Clear();

	ModernEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());

	ModernEngine::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, }, m_SquareColor);
	
	ModernEngine::Renderer2D::EndScene();
}

void Sanbdox2D::OnEvent(ModernEngine::Event& e)
{
	 m_CameraController.OnEvent(e);
}

void Sanbdox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}
