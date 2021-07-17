#pragma once

#include <glm/glm.hpp>

namespace ModernEngine {

	class Camera
	{
	public:
		Camera() = default;

		Camera(const glm::mat4& projectionmatrix)
			: m_ProjectionMatrix(projectionmatrix) {}

		virtual ~Camera() = default;

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

	protected:
		glm::mat4 m_ProjectionMatrix;
	};

}