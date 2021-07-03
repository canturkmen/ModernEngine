#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"

namespace ModernEngine {

	class Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float TilingFactor = 1.0f, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float TilingFactor = 1.0f, const glm::vec4& color = glm::vec4(1.0f));

		static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float TilingFactor = 1.0f, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float TilingFactor = 1.0f, const glm::vec4& color = glm::vec4(1.0f));
	};
}

