#include "mnpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace ModernEngine {

	Shader* Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return new OpenGLShader(filepath);
		}
		}

		MN_CORE_ERROR("Renderer API is not recognized by Modern Engine !");
		return nullptr;
	}

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
			{
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return new OpenGLShader(vertexSrc, fragmentSrc);
			}
		}

		MN_CORE_ERROR("Renderer API is not recognized by Modern Engine !");
		return nullptr;
	}
}
