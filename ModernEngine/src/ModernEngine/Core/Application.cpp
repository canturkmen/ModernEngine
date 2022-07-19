#include "mnpch.h"
#include "Application.h"
#include "Log.h"
#include "ModernEngine/Renderer/Renderer.h"
#include "ModernEngine/Scripting/ScriptEngine.h"

#include <GLFW/glfw3.h>

namespace ModernEngine {

	#define BIND_EVENT_FN(x)  std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_AppInstance = nullptr;

	Application::Application(const ApplicationSpeficiation& specification)
		:m_Specification(specification)
	{
		MN_PROFILE_FUNCTION();

		s_AppInstance = this;

		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);

		m_Window = Window::Create(WindowProps(m_Specification.Name));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		 
		ScriptEngine::Init();
		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		m_LayerStack.PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		MN_PROFILE_FUNCTION();

		ScriptEngine::Shutdown();
		Renderer::ShutDown();
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
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize)); 

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); it++)
		{
			if (e.m_Handled)
				break;

			(*it)->OnEvent(e);
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::Run()
	{
		MN_PROFILE_FUNCTION();

		while (m_Running)
		{
			float time = (float)glfwGetTime();
			DeltaTime deltatime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(deltatime);
			}

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

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}
