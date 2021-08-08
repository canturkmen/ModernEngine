#include "mnpch.h"
#include "SceneCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace ModernEngine {

	SceneCamera::SceneCamera()
	{
		RecalculateProjectionMatrix();
	}


	void SceneCamera::SetPerspective(float perspectiveFOV, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;

		m_PerspectiveFOV = perspectiveFOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;

		RecalculateProjectionMatrix();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;

		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;

		RecalculateProjectionMatrix();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjectionMatrix();
	}

	void SceneCamera::RecalculateProjectionMatrix()
	{
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_ProjectionMatrix = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else
		{
			float Left = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float Right = m_OrthographicSize * m_AspectRatio * 0.5f;
			float Bottom = -m_OrthographicSize * 0.5f;
			float Up = m_OrthographicSize * 0.5f;

			m_ProjectionMatrix = glm::ortho(Left, Right, Bottom, Up, m_OrthographicNear, m_OrthographicFar);
		}
	}
}