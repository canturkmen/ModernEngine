#include "mnpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace ModernEngine {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}
