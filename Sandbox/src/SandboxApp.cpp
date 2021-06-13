#include <ModernEngine.h>

class ExampleLayer : public ModernEngine::Layer
{
public:

	ExampleLayer()
		: Layer("Example Layer") {}

	void OnUpdate() override
	{
		MN_TRACE("Update");
	}

	void OnEvent(ModernEngine::Event& e) override
	{
		MN_TRACE("{0}", e);
	}
};


class Sandbox : public ModernEngine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushLayer(new ModernEngine::ImGuiLayer());
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