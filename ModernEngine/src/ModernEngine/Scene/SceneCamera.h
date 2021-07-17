#pragma once

#include "ModernEngine/Renderer/Camera.h"

namespace ModernEngine {

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);
		float GetOrthographicCameraSize() { return m_OrthographicSize; }
		void SetOrthographicCameraSize(float size) { m_OrthographicSize = size; RecalculateProjectionMatrix();}

	private:
		void RecalculateProjectionMatrix();

	private:
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
		float m_AspectRatio = 0.0f;

	};

}

