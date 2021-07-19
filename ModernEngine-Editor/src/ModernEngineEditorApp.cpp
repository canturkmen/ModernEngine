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
		ModernEngineEditor()
			: Application("Modern Engine Editor")
		{
			PushLayer(new EditorLayer());
		}

		~ModernEngineEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		ModernEngineEditor* editor = new ModernEngineEditor;
		return editor;
	}

}