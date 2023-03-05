#include "mnpch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace ModernEngine {

	namespace Utils {

		static GLenum ModernEngineFormatToGLDataFormat(ImageFormat format)
		{
			switch (format)
			{
				case ModernEngine::ImageFormat::RGB8:		return GL_RGB;
				case ModernEngine::ImageFormat::RGBA8:		return GL_RGBA;
			}

			return 0;
		}

		static GLenum ModernEngineFormatToGLInternalFormat(ImageFormat format)
		{
			switch (format)
			{
				case ModernEngine::ImageFormat::RGB8:		return GL_RGB8;
				case ModernEngine::ImageFormat::RGBA8:		return GL_RGBA8;
			}

			return 0;
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
		: m_Specification(specification), m_Width(m_Specification.Width), m_Height(m_Specification.Height)
	{
		m_InternalFormat = Utils::ModernEngineFormatToGLInternalFormat(m_Specification.Format);
		m_DataFormat = Utils::ModernEngineFormatToGLDataFormat(m_Specification.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath)
		:m_FilePath(filepath)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		m_Height = height;
		m_Width = width;

		GLenum internalformat = 0, dataformat = 0;
		if (channels == 4)
		{
			internalformat = GL_RGBA8;
			dataformat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalformat = GL_RGB8;
			dataformat = GL_RGB;
		}

		m_InternalFormat = internalformat;
		m_DataFormat = dataformat;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalformat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataformat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		MN_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		MN_PROFILE_FUNCTION();

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		MN_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}
}
