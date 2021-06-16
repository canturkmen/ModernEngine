#include "mnpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace ModernEngine {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:
			{
				return nullptr;
			}

			case RendererAPI::OpenGL:
			{
				return new OpenGLVertexBuffer(vertices, size);
			}
		}

		MN_CORE_ERROR("Renderer API is not recognized by Modern Engine !");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:
			{
				return nullptr;
			}

			case RendererAPI::OpenGL:
			{
				return new OpenGLIndexBuffer(indices, count);
			}
		}

		MN_CORE_ERROR("Renderer API is not recognized by Modern Engine !");
		return nullptr;
	}
}
