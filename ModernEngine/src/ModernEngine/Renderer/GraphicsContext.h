#pragma once

namespace ModernEngine {

	// This is an interface for other graphics context classes such as OpenGL, DirextX and Vulkan
	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}
