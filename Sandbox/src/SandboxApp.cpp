#include "ModernEngine.h"
#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public ModernEngine::Layer
{
public:
	ExampleLayer()
		: Layer("Example Layer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		m_VertexArray.reset(ModernEngine::VertexArray::Create());

		ModernEngine::Ref<ModernEngine::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset((ModernEngine::VertexBuffer::Create(vertices, sizeof(vertices))));

		ModernEngine::BufferLayout layout =
		{
			{ModernEngine::ShaderDataType::Float3, "a_Position"},
			{ModernEngine::ShaderDataType::Float4, "v_Color"}
		};

		m_VertexBuffer->SetBufferLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		ModernEngine::Ref<ModernEngine::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(ModernEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
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

		m_Shader.reset(ModernEngine::Shader::Create(vertexSrc, fragmentSrc));

		float rectangeVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		m_RectangleVertexArray.reset(ModernEngine::VertexArray::Create());

		ModernEngine::Ref<ModernEngine::VertexBuffer> m_RectangleVertexBuffer;
		m_RectangleVertexBuffer.reset(ModernEngine::VertexBuffer::Create(rectangeVertices, sizeof(rectangeVertices)));

		ModernEngine::BufferLayout rectangleLayout =
		{
			{ModernEngine::ShaderDataType::Float3, "a_Position"},
		};

		m_RectangleVertexBuffer->SetBufferLayout(rectangleLayout);
		m_RectangleVertexArray->AddVertexBuffer(m_RectangleVertexBuffer);

		uint32_t rectangleIndices[6] = { 0, 1, 2, 2, 3, 0 };
		ModernEngine::Ref<ModernEngine::IndexBuffer> m_RectangeIndexBuffer;
		m_RectangeIndexBuffer.reset(ModernEngine::IndexBuffer::Create(rectangleIndices, sizeof(rectangleIndices) / sizeof(uint32_t)));
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

		m_FlatColorShader.reset(ModernEngine::Shader::Create(flatColorVertexShader, flatColorFragmentShader));
	}

	void OnUpdate(ModernEngine::DeltaTime dt) override
	{
		if (ModernEngine::Input::IsKeyPressed(MN_KEY_W))
			m_CameraPosition.y += m_CameraMovingSpeed * dt;
		else if (ModernEngine::Input::IsKeyPressed(MN_KEY_S))
			m_CameraPosition.y -= m_CameraMovingSpeed * dt;

		if (ModernEngine::Input::IsKeyPressed(MN_KEY_D))
			m_CameraPosition.x += m_CameraMovingSpeed * dt;
		else if (ModernEngine::Input::IsKeyPressed(MN_KEY_A))
			m_CameraPosition.x -= m_CameraMovingSpeed * dt;

		if (ModernEngine::Input::IsKeyPressed(MN_KEY_LEFT))
			m_CameraRotation += m_CameraRotationSpeed * dt;
		if (ModernEngine::Input::IsKeyPressed(MN_KEY_RIGHT))
			m_CameraRotation -= m_CameraRotationSpeed * dt;

		ModernEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		ModernEngine::RenderCommand::Clear();

		ModernEngine::Renderer::BeginScene(m_Camera);

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

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

		// ModernEngine::Renderer::Submit(m_VertexArray, m_Shader);

		ModernEngine::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

private:
	ModernEngine::Ref<ModernEngine::VertexArray> m_VertexArray;
	ModernEngine::Ref<ModernEngine::Shader> m_Shader;

	ModernEngine::Ref<ModernEngine::VertexArray> m_RectangleVertexArray;
	ModernEngine::Ref<ModernEngine::Shader> m_FlatColorShader;

	ModernEngine::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
	float m_CameraMovingSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.0f };
};

class Sandbox : public ModernEngine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
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