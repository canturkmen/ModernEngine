#include <ModernEngine.h>
#include <ModernEngine/Core/EntryPoint.h>

#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "EditorLayer.h"

namespace ModernEngine {

	class ModernEngineEditor : public Application
	{
	public:
		ModernEngineEditor(const ApplicationSpeficiation& spec)
			: Application(spec)
		{
			PushLayer(new EditorLayer());
		}
	};

	Application* CreateApplication(ModernEngine::ApplicationCommandLineArgs args)
	{
		ApplicationSpeficiation spec;
		spec.Name = "ModernEngine-Editor";
		spec.CommandLineArgs = args;

		return new ModernEngineEditor(spec);
	}

}