#include "mnpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace ModernEngine {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:
			{
				return nullptr;
			}

			case RendererAPI::OpenGL:
			{
				return new OpenGLVertexArray();
			}
		}

		MN_CORE_ERROR("Renderer API is not recognized by Modern Engine !");
		return nullptr;
	}
}