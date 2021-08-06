#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"

#include "Shader.h"

namespace ModernEngine {

	class Renderer
	{
	public:

		static void OnWindowResize(uint32_t width, uint32_t height);
		static void Init();
		static void BeginScene(const OrthographicCamera& camera);
		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));
		static void EndScene();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;
	};
}

