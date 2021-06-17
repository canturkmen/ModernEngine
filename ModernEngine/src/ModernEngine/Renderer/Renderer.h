#pragma once

#include "RenderCommand.h"

namespace ModernEngine {

	class Renderer
	{
	public:
		static void BeginScene();
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
		static void EndScene();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}

