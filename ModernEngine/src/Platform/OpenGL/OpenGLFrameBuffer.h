#pragma once

#include "ModernEngine/Renderer/FrameBuffer.h"

namespace ModernEngine {

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		void Invalidate();
		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixels(uint32_t attachmentIndex, int x, int y) override;
		virtual void ClearAttachments(uint32_t attachmentIndex, int data) override;

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual const FrameBufferSpecification& GetSpefications() const override { return m_Specification; }
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { return m_ColorAttachments[index];}


	private:
		uint32_t m_RendererID;
		FrameBufferSpecification m_Specification; 

		std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecifications;
		FrameBufferTextureSpecification m_DepthAttachmentSpecification = FrameBufferTextureFormat::None;
		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;
	};
}


