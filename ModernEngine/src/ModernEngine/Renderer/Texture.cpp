#include "mnpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace ModernEngine {
	
	Ref<Texture2D> Texture2D::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
			{
				return nullptr;
			}
			
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLTexture2D>(filepath);
			}
		}
	}

}