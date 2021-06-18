#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"

#include "Shader.h"

namespace ModernEngine {

	class Renderer
	{
	public:
		static void BeginScene(const OrthographicCamera& camera);
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));
		static void EndScene();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}

