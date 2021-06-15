#include "mnpch.h"
#include "OpenGLContext.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace ModernEngine {

	ModernEngine::OpenGLContext::OpenGLContext(GLFWwindow* windowhandle)
		: m_WindowHandle(windowhandle)
	{
		MN_CORE_ASSERT(windowhandle, "Window Handle is Null");
	}

	void ModernEngine::OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MN_CORE_ASSERT(status, "Failed to initialize Glad ");
	}

	void ModernEngine::OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}

