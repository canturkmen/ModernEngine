#pragma once

#include "Core.h"
#include "Window.h"
#include "ModernEngine/Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "DeltaTime.h"
#include "ModernEngine/ImGui/ImGuiLayer.h"

namespace ModernEngine {

	class Application
	{
	public:
		Application();
		virtual ~Application() = default;
	
		void Run(); 
		void OnEvent(Event& e);
		void Close();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_AppInstance; }

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		bool OnCloseWindow(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_AppInstance;
	};

	Application* CreateApplication();
}

