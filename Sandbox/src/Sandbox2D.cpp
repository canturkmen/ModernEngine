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
	m_SquareVA.reset(ModernEngine::VertexArray::Create());

	float SquareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	ModernEngine::Ref<ModernEngine::VertexBuffer> SquareVertexBuffer;
	SquareVertexBuffer.reset(ModernEngine::VertexBuffer::Create(SquareVertices, sizeof(SquareVertices)));

	ModernEngine::BufferLayout SquareLayout =
	{
		{ModernEngine::ShaderDataType::Float3, "a_Position"},
	};

	SquareVertexBuffer->SetBufferLayout(SquareLayout);
	m_SquareVA->AddVertexBuffer(SquareVertexBuffer);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	ModernEngine::Ref<ModernEngine::IndexBuffer> SquareIB;
	SquareIB.reset(ModernEngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(SquareIB);

	m_FlatColorShader = ModernEngine::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sanbdox2D::OnDetach()
{

}

void Sanbdox2D::OnUpdate(ModernEngine::DeltaTime dt)
{
	m_CameraController.OnUpdate(dt);

	ModernEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	ModernEngine::RenderCommand::Clear();

	ModernEngine::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<ModernEngine::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<ModernEngine::OpenGLShader>(m_FlatColorShader)->UploadShaderFloat4("u_Color", m_SquareColor);

	ModernEngine::Renderer::Submit(m_SquareVA, m_FlatColorShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	ModernEngine::Renderer::EndScene();
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
