#include "mnpch.h"
#include "Application.h"
#include "Log.h"
#include "Renderer/Renderer.h"

namespace ModernEngine {

	#define BIND_EVENT_FN(x)  std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_AppInstance = nullptr;
	

	Application::Application()
	{
		s_AppInstance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		m_LayerStack.PushOverlay(m_ImGuiLayer);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		m_VertexArray.reset(VertexArray::Create());
		
		std::shared_ptr<VertexBuffer> m_VertexBuffer; 
		m_VertexBuffer.reset((VertexBuffer::Create(vertices, sizeof(vertices))));

		BufferLayout layout =
		{
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "v_Color"}
		};

		m_VertexBuffer->SetBufferLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc = R"(
				#version 330 core
			
				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;		
		
				out vec3 v_Position; 
				out vec4 v_Color;

				void main()
				{
					v_Position = a_Position;
					v_Color = a_Color;
					gl_Position = vec4(a_Position, 1.0);
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

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		float rectangeVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		m_RectangleVertexArray.reset(VertexArray::Create());

		std::shared_ptr<VertexBuffer> m_RectangleVertexBuffer;
		m_RectangleVertexBuffer.reset(VertexBuffer::Create(rectangeVertices, sizeof(rectangeVertices)));

		BufferLayout rectangleLayout =
		{
			{ShaderDataType::Float3, "a_Position"},
		};

		m_RectangleVertexBuffer->SetBufferLayout(rectangleLayout);
		m_RectangleVertexArray->AddVertexBuffer(m_RectangleVertexBuffer);


		uint32_t rectangleIndices[8] = { 0, 1, 2, 2, 3, 0};
		std::shared_ptr<IndexBuffer> m_RectangeIndexBuffer;
		m_RectangeIndexBuffer.reset(IndexBuffer::Create(rectangleIndices, sizeof(rectangleIndices) / sizeof(uint32_t)));
		m_RectangleVertexArray->SetIndexBuffer(m_RectangeIndexBuffer);

		std::string rectangleVertexSrc = R"(
				#version 330 core
			
				layout(location = 0) in vec3 a_Position;	
		
				out vec3 v_Position; 

				void main()
				{
					v_Position = a_Position;
					gl_Position = vec4(a_Position, 1.0);
				}
			)";

		std::string rectangleFragmentSrc = R"(
				#version 330 core

				layout(location = 0) out vec4 color;

				in vec3 v_Position;

				void main()
				{
					color = vec4(0.2, 0.8, 0.5, 1.0);
				}
			)";

		m_RectangleShader.reset(new Shader(rectangleVertexSrc, rectangleFragmentSrc));
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnCloseWindow));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();

			Renderer::BeginScene();

			m_RectangleShader->Bind();
			Renderer::Submit(m_RectangleVertexArray);

			m_Shader->Bind();
			Renderer::Submit(m_VertexArray);

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnCloseWindow(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
