#include "mnpch.h"
#include "OrthographicCameraController.h"

#include "Input.h"
#include "KeyCodes.h"

namespace ModernEngine {

	OrthographicCameraController::OrthographicCameraController(float aspectratio, bool enablerotation)
		: m_AspectRatio(aspectratio), m_OrthographicCamera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_CameraRotationEnabled(enablerotation)
	{

	}

	void OrthographicCameraController::OnUpdate(DeltaTime dt)
	{
		if (ModernEngine::Input::IsKeyPressed(MN_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * dt;
		else if (ModernEngine::Input::IsKeyPressed(MN_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * dt;

		if (ModernEngine::Input::IsKeyPressed(MN_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * dt;
		else if (ModernEngine::Input::IsKeyPressed(MN_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * dt;

		if (m_CameraRotationEnabled)
		{
			if (ModernEngine::Input::IsKeyPressed(MN_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * dt;
			if (ModernEngine::Input::IsKeyPressed(MN_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * dt;

			m_OrthographicCamera.SetRotation(m_CameraRotation);
		}

		m_OrthographicCamera.SetPosition(m_CameraPosition);
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(MN_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(MN_BIND_EVENT_FN(OrthographicCameraController::OnWindowResize));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_OrthographicCamera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_OrthographicCamera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}