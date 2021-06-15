#pragma once

#include "Core.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "Input.h"
#include "KeyCodes.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"

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

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_AppInstance; }

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool OnCloseWindow(WindowCloseEvent& e);

		LayerStack m_LayerStack;

		unsigned int m_VertexArray;
		unsigned int m_VertexBuffer;
		unsigned int m_IndexBuffer;

	private:
		static Application* s_AppInstance;
	};

	Application* CreateApplication();
}

