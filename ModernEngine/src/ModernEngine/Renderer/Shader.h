#pragma once

#include "glm/glm.hpp"

namespace ModernEngine {

	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~Shader();

		void Bind() const;
		void UnBind() const;

		void UploadShaderMat4(const std::string& name, const glm::mat4& ViewProjectionMatrix);

	private:
		uint32_t m_RendererID;
	};
}

