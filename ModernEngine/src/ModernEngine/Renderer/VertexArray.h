#pragma once

#include "ModernEngine/Renderer/Buffer.h"

namespace ModernEngine {

	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexbuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexbuffer) = 0;

		static Ref<VertexArray> Create();
	};
}

