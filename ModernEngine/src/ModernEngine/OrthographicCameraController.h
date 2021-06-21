#pragma once

#include "Core/DeltaTime.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

#include "Renderer/OrthographicCamera.h"

namespace ModernEngine {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectratio, bool enablerotation);
		
		void OnEvent(Event& e);
		void OnUpdate(DeltaTime dt);
		
		OrthographicCamera& GetCamera() { return m_OrthographicCamera; }
		const OrthographicCamera& GetCamera() const { return m_OrthographicCamera; }


	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_OrthographicCamera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f; 
		float m_CameraRotationSpeed = 180.0f;
		float m_CameraTranslationSpeed = 5.0f;
		
		bool m_CameraRotationEnabled;
	};

}
