#pragma once

#include "ModernEngine/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace ModernEngine {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowhandle);

		void Init() override;
		void SwapBuffers() override;

	private: 
		GLFWwindow* m_WindowHandle;
	};

}

