#pragma once

#include "RendererAPI.h"

namespace ModernEngine {

	class RenderCommand
	{
	public:
		inline static void OnWindowResize(uint32_t width, uint32_t height) { s_RendererAPI->SetViewport(0, 0, width, height); }
		inline static void Init() { s_RendererAPI->Init(); }
		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
		inline static void Clear() { s_RendererAPI->Clear(); }
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) { s_RendererAPI->DrawIndexed(vertexArray, indexCount); }

	private:
		static Scope<RendererAPI> s_RendererAPI; 
	};
}
