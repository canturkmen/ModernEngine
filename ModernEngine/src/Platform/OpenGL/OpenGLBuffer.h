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


	private:
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

