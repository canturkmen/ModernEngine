#pragma once

#include "ModernEngine/Renderer/Shader.h"
#include "glm/glm.hpp"

namespace ModernEngine {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		void UploadShaderInt(const std::string& name, int value);
		void UploadShaderFloat(const std::string& name, float value);
		void UploadShaderFloat2(const std::string& name, const glm::vec2& value);
		void UploadShaderFloat3(const std::string& name, const glm::vec3& value);
		void UploadShaderFloat4(const std::string& name, const glm::vec4& value);
		void UploadShaderMat3(const std::string& name, const glm::mat3& matrix);
		void UploadShaderMat4(const std::string& name, const glm::mat4& matrix);

	private:
		uint32_t m_RendererID;
	};


}
