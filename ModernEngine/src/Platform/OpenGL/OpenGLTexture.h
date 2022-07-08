#pragma once

#include "ModernEngine/Renderer/Texture.h"
#include <glad/glad.h>

namespace ModernEngine {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& filepath);
		~OpenGLTexture2D();

		virtual void SetData(void* data, uint32_t size) override;

		virtual uint32_t GetWidth()  const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual const std::string& GetFilePath() const override { return m_FilePath; }

		virtual void Bind(uint32_t slot) const override;

	private:
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
		std::string m_FilePath;
		GLenum m_InternalFormat, m_DataFormat;
	};
}