#pragma once

#include "Base.h"
#include "Window.h"
#include "ModernEngine/Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "DeltaTime.h"
#include "ModernEngine/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace ModernEngine {

	class Application
	{
	public:
		Application(const std::string& name = "Modern Engine App");
		virtual ~Application() = default;
 
		void OnEvent(Event& e);
		void Close();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		Window& GetWindow() { return *m_Window; }
		static Application& Get() { return *s_AppInstance; }

	private:
		void Run();
		bool OnCloseWindow(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_AppInstance;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication();
}

