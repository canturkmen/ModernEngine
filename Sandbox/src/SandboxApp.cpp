#include "ModernEngine.h"
#include "imgui/imgui.h"

class ExampleLayer : public ModernEngine::Layer
{
public:
	ExampleLayer()
		:Layer("Example Layer")
	{

	}

	virtual void OnUpdate() override
	{

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello");
		ImGui::End();
	}
};

class Sandbox : public ModernEngine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer);
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