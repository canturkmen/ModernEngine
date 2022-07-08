#include <ModernEngine.h>
#include <ModernEngine/Core/EntryPoint.h>

#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class Sandbox : public ModernEngine::Application
{
public:
	Sandbox(const ModernEngine::ApplicationSpeficiation& specification)
		:ModernEngine::Application(specification)
	{
		PushLayer(new Sanbdox2D());
	}

	~Sandbox()
	{

	}
};

ModernEngine::Application* ModernEngine::CreateApplication(ModernEngine::ApplicationCommandLineArgs args)
{
	ApplicationSpeficiation spec;
	spec.Name = "Sandbox";
	spec.WorkingDirectory = "../ModernEngine-Editor";
	spec.CommandLineArgs = args;
	return new Sandbox(spec);
}