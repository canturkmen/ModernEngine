#pragma once

#include "ModernEngine/Renderer/Buffer.h"

namespace ModernEngine {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		inline virtual void SetBufferLayout(const BufferLayout& layout) override { m_BufferLayout = layout; }
		inline virtual const BufferLayout& GetBufferLayout() const override { return m_BufferLayout; }

	private:
		BufferLayout m_BufferLayout;
		uint32_t m_RendererID;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		inline virtual uint32_t GetCount() const { return m_Count; }

		virtual void Bind() const;
		virtual void UnBind() const;

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

}

