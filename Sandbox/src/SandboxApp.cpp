#include <ModernEngine.h>
#include <ModernEngine/Core/EntryPoint.h>

#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class ExampleLayer : public ModernEngine::Layer
{
public:
	ExampleLayer()
		: Layer("Example Layer"), m_CameraController(1280.0f / 720.0f)
	{
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		m_VertexArray = ModernEngine::VertexArray::Create();

		ModernEngine::Ref<ModernEngine::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer = ModernEngine::VertexBuffer::Create(vertices, sizeof(vertices));

		ModernEngine::BufferLayout layout =
		{
			{ModernEngine::ShaderDataType::Float3, "a_Position"},
			{ModernEngine::ShaderDataType::Float4, "v_Color"}
		};

		m_VertexBuffer->SetBufferLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		ModernEngine::Ref<ModernEngine::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer = ModernEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc = R"(
				#version 330 core
			
				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;		
		
				out vec3 v_Position; 
				out vec4 v_Color;

				uniform mat4 u_ViewProjectionMatrix;
				uniform mat4 u_Transform;

				void main()
				{
					v_Position = a_Position;
					v_Color = a_Color;
					gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
				}
			)";

		std::string fragmentSrc = R"(
				#version 330 core

				layout(location = 0) out vec4 color;

				in vec3 v_Position;
				in vec4 v_Color;	

				void main()
				{
					color = vec4(v_Position + 0.5, 0.5);
					color = v_Color;
				}
			)";

		m_TriangleShader = ModernEngine::Shader::Create("TriangleShader", vertexSrc, fragmentSrc);

		m_RectangleVertexArray = ModernEngine::VertexArray::Create();

		float rectangeVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		ModernEngine::Ref<ModernEngine::VertexBuffer> m_RectangleVertexBuffer;
		m_RectangleVertexBuffer = ModernEngine::VertexBuffer::Create(rectangeVertices, sizeof(rectangeVertices));

		ModernEngine::BufferLayout rectangleLayout =
		{
			{ModernEngine::ShaderDataType::Float3, "a_Position"},
			{ModernEngine::ShaderDataType::Float2, "a_Textcoord"}
		};

		m_RectangleVertexBuffer->SetBufferLayout(rectangleLayout);
		m_RectangleVertexArray->AddVertexBuffer(m_RectangleVertexBuffer);

		uint32_t rectangleIndices[6] = { 0, 1, 2, 2, 3, 0 };
		ModernEngine::Ref<ModernEngine::IndexBuffer> m_RectangeIndexBuffer;
		m_RectangeIndexBuffer = ModernEngine::IndexBuffer::Create(rectangleIndices, sizeof(rectangleIndices) / sizeof(uint32_t));
		m_RectangleVertexArray->SetIndexBuffer(m_RectangeIndexBuffer);

		std::string flatColorVertexShader = R"(
				#version 330 core
			
				layout(location = 0) in vec3 a_Position;	
		
				out vec3 v_Position; 

				uniform mat4 u_ViewProjectionMatrix;
				uniform mat4 u_Transform;

				void main()
				{
					v_Position = a_Position;
					gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
				}
			)";

		std::string flatColorFragmentShader = R"(
				#version 330 core

				layout(location = 0) out vec4 color;

				in vec3 v_Position;

				uniform vec3 u_Color;

				void main()
				{
					color = vec4(u_Color, 1.0);
				}
			)";

		m_FlatColorShader = ModernEngine::Shader::Create("FlatColorShader", flatColorVertexShader, flatColorFragmentShader);

		std::string TextureShaderVertexShader = R"(
				#version 330 core
			
				layout(location = 0) in vec3 a_Position;	
				layout(location = 1) in vec2 a_TexCoord;
		
				out vec2 v_TexCoord;

				uniform mat4 u_ViewProjectionMatrix;
				uniform mat4 u_Transform;

				void main()
				{
					v_TexCoord = a_TexCoord;
					gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
				}
			)";

		std::string TextureShaderFragmentShader = R"(
				#version 330 core

				layout(location = 0) out vec4 color;

				in vec2 v_TexCoord;

				uniform sampler2D u_Texture;

				void main()
				{
					color = texture(u_Texture, v_TexCoord);
				}
			)";

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");	

		m_Texture = ModernEngine::Texture2D::Create("assets/textures/Checkerboard.png");

		std::dynamic_pointer_cast<ModernEngine::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<ModernEngine::OpenGLShader>(textureShader)->UploadShaderInt("u_Texture", 0);
	}

	virtual void OnUpdate(ModernEngine::DeltaTime dt) override
	{
		m_CameraController.OnUpdate(dt);

		ModernEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		ModernEngine::RenderCommand::Clear();

		ModernEngine::Renderer::BeginScene(m_CameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		glm::vec4 redColor({ 0.8, 0.3, 0.2, 1.0 });
		glm::vec4 greenColor({ 0.2, 0.8, 0.5, 1.0 });

		std::dynamic_pointer_cast<ModernEngine::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<ModernEngine::OpenGLShader>(m_FlatColorShader)->UploadShaderFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 10; y++)
		{
			for (int x = 0; x < 10; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				ModernEngine::Renderer::Submit(m_RectangleVertexArray, m_FlatColorShader, transform);
			}
		}

		auto textureUpdateShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		ModernEngine::Renderer::Submit(m_RectangleVertexArray, textureUpdateShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		ModernEngine::Renderer::Submit(m_VertexArray, m_TriangleShader, glm::translate(glm::mat4(1.0f), glm::vec3(1.2f, 1.2f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));

		ModernEngine::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	virtual void OnEvent(ModernEngine::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

private:
	ModernEngine::Ref<ModernEngine::VertexArray> m_VertexArray;
	ModernEngine::Ref<ModernEngine::VertexArray> m_RectangleVertexArray;
	ModernEngine::Ref<ModernEngine::Shader> m_TriangleShader, m_FlatColorShader;
	ModernEngine::ShaderLibrary m_ShaderLibrary;

	ModernEngine::Ref<ModernEngine::Texture2D> m_Texture, m_NetflixLogoTexture;

	ModernEngine::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.0f };
};

class Sandbox : public ModernEngine::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sanbdox2D());
	}

	~Sandbox()
	{

	}
};

ModernEngine::Application* ModernEngine::CreateApplication()
{
	Sandbox* sandbox = new Sandbox;
	return sandbox;
}