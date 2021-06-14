#include <ModernEngine.h>


class Sandbox : public ModernEngine::Application
{
public:
	Sandbox()
	{
		
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