#include <ModernEngine.h>


class Sandbox : public ModernEngine::Application
{
public:
	Sandbox()
	{
		PushOverlay(new ModernEngine::ImGuiLayer());
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