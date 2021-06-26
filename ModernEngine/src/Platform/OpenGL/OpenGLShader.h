#pragma once

#include "ModernEngine/Renderer/Shader.h"
#include "glm/glm.hpp"

typedef unsigned int GLenum;

namespace ModernEngine {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		virtual const std::string& GetName() const override { return m_Name; }

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void SetInt(const std::string& name, int value);
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value);
		virtual void SetFloat4(const std::string& name, const glm::vec4& value);
		virtual void SetMat4(const std::string& name, const glm::mat4& value);

		void UploadShaderInt(const std::string& name, int value);
		void UploadShaderFloat(const std::string& name, float value);
		void UploadShaderFloat2(const std::string& name, const glm::vec2& value);
		void UploadShaderFloat3(const std::string& name, const glm::vec3& value);
		void UploadShaderFloat4(const std::string& name, const glm::vec4& value);
		void UploadShaderMat3(const std::string& name, const glm::mat3& matrix);
		void UploadShaderMat4(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		std::string m_Name;
		uint32_t m_RendererID;
	};


}
