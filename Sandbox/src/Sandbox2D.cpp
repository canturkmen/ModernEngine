#include "mnpch.h"
#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include <chrono>

template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		: m_Name(name), m_Func(func), m_Stopped(false)
	{
		m_TimeStartingPoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop()
	{
		std::chrono::time_point<std::chrono::steady_clock> TimeEndingPoint = std::chrono::high_resolution_clock::now();
		int start = std::chrono::time_point_cast<std::chrono::microseconds>(m_TimeStartingPoint).time_since_epoch().count();
		int end = std::chrono::time_point_cast<std::chrono::microseconds>(TimeEndingPoint).time_since_epoch().count();
		m_Stopped = true;

		float duration = (end - start) * 0.001f;

		m_Func({ m_Name, duration});
	}

private:
	const char* m_Name;
	Fn m_Func;
	bool m_Stopped;
	std::chrono::time_point<std::chrono::steady_clock> m_TimeStartingPoint;
};

#define SCOPE_PROFILE(name)	Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

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
	{
		SCOPE_PROFILE("On Update");
		m_CameraController.OnUpdate(dt);
	}

	{
		SCOPE_PROFILE("Rendering Preparing");
		ModernEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		ModernEngine::RenderCommand::Clear();
		ModernEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	}

	{
		SCOPE_PROFILE("Rendering");
		ModernEngine::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_SquareColor);
		ModernEngine::Renderer2D::DrawQuad({ 2.0f, -1.0f, 0.0f }, { 0.5f, 1.0f }, m_SquareColor);
		ModernEngine::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_Checkerboard);
		ModernEngine::Renderer2D::EndScene();
	}
}

void Sanbdox2D::OnEvent(ModernEngine::Event& e)
{
	 m_CameraController.OnEvent(e);
}

void Sanbdox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

	for(auto& result : m_ProfileResults)
	{
		char label[50];
		strcpy(label, "  %.3fms ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}
	m_ProfileResults.clear();

	ImGui::End();
}
