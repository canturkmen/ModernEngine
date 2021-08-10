#pragma once

#include "Camera.h"
#include "ModernEngine/Events/Event.h"
#include "ModernEngine/Events/MouseEvent.h"
#include "ModernEngine/Core/DeltaTime.h"
#include <glm/glm.hpp>

namespace ModernEngine {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float perspectiveFOV, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(DeltaTime dt);
		void OnEvent(Event& e);
		void SetViewport(float width, float height) { m_ViewportHeight = height; m_ViewportWidth = width; UpdateProjectionMatrix(); }

		float GetDistance() const { return m_Distance; }
		void SetDistance(float distance) { m_Distance = distance; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		glm::quat GetOrientation() const;
		glm::mat4 GetViewProjectionMatrix() { return m_ProjectionMatrix * m_ViewMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

	private:
		void UpdateProjectionMatrix();
		void UpdateViewMatrix();

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> MoveSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

		void MouseMove(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		bool OnMouseScroll(MouseScrolledEvent& e);
	private:

		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_PerspectiveFOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.01f, m_FarClip = 1000.0f;
		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;
		float m_ViewportWidth = 1280.0f, m_ViewportHeight = 800.0f;
	};

}

