#pragma once

#include "Core.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"

namespace ModernEngine {

	class MN_API Application
	{
	public:
		Application();
		virtual ~Application();
	
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		bool CloseWindowsWindow(WindowCloseEvent e);

		LayerStack m_LayerStack;
	};

	Application* CreateApplication();
}

