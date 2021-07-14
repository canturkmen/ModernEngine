#include "mnpch.h"
#include "ModernEngine/Core/Input.h"
#include "ModernEngine/Core/Application.h"
#include "GLFW/glfw3.h"

namespace ModernEngine {

	bool Input::IsKeyPressed(int key)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, key);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
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