#pragma once

#include "ModernEngine/Renderer/Camera.h"

namespace ModernEngine {

	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1};

	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float perspectiveFOV, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetPerspectiveCameraFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveCameraFOV(float FOV) { m_PerspectiveFOV = FOV; RecalculateProjectionMatrix(); }

		float GetPerspectiveCameraNear() const { return m_PerspectiveNear; }
		void SetPerspectiveCameraNear(float perspectivenear) { m_PerspectiveNear = perspectivenear; RecalculateProjectionMatrix(); }

		float GetPerspectiveCameraFar() const { return m_PerspectiveFar; }
		void SetPerspectiveCameraFar(float perspectivefar) { m_PerspectiveFar = perspectivefar; RecalculateProjectionMatrix(); }

		float GetOrthographicCameraSize() const { return m_OrthographicSize; }
		void SetOrthographicCameraSize(float size) { m_OrthographicSize = size; RecalculateProjectionMatrix();}

		float GetOrthographicCameraNear() const { return m_OrthographicNear; }
		void SetOrthographicCameraNear(float orthographicnear) { m_OrthographicNear = orthographicnear; RecalculateProjectionMatrix(); }

		float GetOrthographicCameraFar() const { return m_OrthographicFar; }
		void SetOrthographicCameraFar(float orthographicfar) { m_OrthographicFar = orthographicfar; RecalculateProjectionMatrix(); }

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; }

	private:
		void RecalculateProjectionMatrix();

	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_PerspectiveFOV = glm::radians(45.0f); 
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_AspectRatio = 0.0f;
	};

}

