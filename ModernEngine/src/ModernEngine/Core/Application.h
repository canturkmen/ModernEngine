#pragma once

#include "Base.h"
#include "Window.h"
#include "ModernEngine/Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "DeltaTime.h"
#include "ModernEngine/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace ModernEngine {

	struct ApplicationCommandLineArgs 
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			return Args[index];
		}
	}; 

	struct ApplicationSpeficiation
	{
		std::string Name = "Modern Engine Application";
		std::string WorkingDirectory;
		ApplicationCommandLineArgs CommandLineArgs;
	};

	class Application
	{
	public:
		Application(const ApplicationSpeficiation& specification);
		virtual ~Application();
 
		void OnEvent(Event& e);
		void Close();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		Window& GetWindow() { return *m_Window; }
		static Application& Get() { return *s_AppInstance; }

		const ApplicationSpeficiation& GetSpecification() const { return m_Specification; }

	private:
		void Run();
		bool OnCloseWindow(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		ApplicationSpeficiation m_Specification;
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

	Application* CreateApplication(ApplicationCommandLineArgs args);
}

