#include "mnpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace ModernEngine {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())

		{
			case RendererAPI::API::None:
			{
				return nullptr;
			}

			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLVertexBuffer>(size);
			}
		}
		MN_CORE_ERROR("Renderer API is not recognized by Modern Engine !");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
			{
				return nullptr;
			}

			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLVertexBuffer>(vertices, size);
			}
		}

		MN_CORE_ERROR("Renderer API is not recognized by Modern Engine !");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
			{
				return nullptr;
			}

			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLIndexBuffer>(indices, count);
			}
		}

		MN_CORE_ERROR("Renderer API is not recognized by Modern Engine !");
		return nullptr;
	}
}
