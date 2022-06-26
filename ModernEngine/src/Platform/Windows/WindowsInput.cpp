#include "mnpch.h"
#include "ModernEngine/Core/Input.h"
#include "ModernEngine/Core/Application.h"
#include "GLFW/glfw3.h"

namespace ModernEngine {

	bool Input::IsKeyPressed(KeyCode key)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<uint32_t>(key));
		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<uint32_t>(button));
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { (float)xPos, (float)yPos };
	}

	float Input::GetMouseXPosition()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseYPosition()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}
}