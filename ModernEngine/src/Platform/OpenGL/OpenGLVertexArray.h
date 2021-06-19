#pragma once

#include "ModernEngine/Renderer/VertexArray.h"

namespace ModernEngine {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void UnBind() const override; 

		inline virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		inline virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBufer; }

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexbuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexbuffer) override;

	private:
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBufer;
		uint32_t m_RendererID;
	};

}

