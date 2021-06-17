#include "mnpch.h"
#include "OpenGLContext.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace ModernEngine {

	OpenGLContext::OpenGLContext(GLFWwindow* windowhandle)
		: m_WindowHandle(windowhandle)
	{
		MN_CORE_ASSERT(windowhandle, "Window Handle is Null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MN_CORE_ASSERT(status, "Failed to initialize Glad ");

		MN_CORE_INFO("GPU Info: {0}", glGetString(GL_VENDOR));
		MN_CORE_INFO("Renderer Info: {0}", glGetString(GL_RENDERER));
		MN_CORE_INFO("Version Info: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}

