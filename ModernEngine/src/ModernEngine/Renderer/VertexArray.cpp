#include "mnpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace ModernEngine {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
			{
				return nullptr;
			}

			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLVertexArray>();
			}
		}

		MN_CORE_ERROR("Renderer API is not recognized by Modern Engine !");
		return nullptr;
	}
}