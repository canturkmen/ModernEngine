#include "mnpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace ModernEngine {

	struct Renderer2DStorage
	{
		Ref<VertexArray> SquareVA;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		s_Data->SquareVA.reset(VertexArray::Create());

		float SquareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Ref<VertexBuffer> SquareVertexBuffer;
		SquareVertexBuffer.reset(VertexBuffer::Create(SquareVertices, sizeof(SquareVertices)));

		BufferLayout SquareLayout =
		{
			{ShaderDataType::Float3, "a_Position"},
		};

		SquareVertexBuffer->SetBufferLayout(SquareLayout);
		s_Data->SquareVA->AddVertexBuffer(SquareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		ModernEngine::Ref<IndexBuffer> SquareIB;
		SquareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_Data->SquareVA->SetIndexBuffer(SquareIB);

		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	}
	

	void Renderer2D::ShutDown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadShaderMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadShaderMat4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{
		
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadShaderFloat4("u_Color", color);

		s_Data->SquareVA->Bind();
		RenderCommand::DrawIndexed(s_Data->SquareVA);
	}

}