#pragma once

#include <glm/glm.hpp>

namespace ModernEngine {

	class Camera
	{
	public:
		Camera(const glm::mat4& projectionmatrix)
			: m_ProjectionMatrix(projectionmatrix) {}

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

	private:
		glm::mat4 m_ProjectionMatrix;
	};

}