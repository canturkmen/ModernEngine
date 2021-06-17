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

		inline virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		inline virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBufer; }

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexbuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexbuffer) override;

	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBufer;
		uint32_t m_RendererID;
	};

}

