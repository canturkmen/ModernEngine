#include "mnpch.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace ModernEngine {

	RendererAPI::API RendererAPI::s_API = API::OpenGL;
	
	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None :
			{
				return nullptr;
			}

			case RendererAPI::API::OpenGL:
			{
				return CreateScope<OpenGLRendererAPI>();
			}
		}

		return nullptr;
	}
}