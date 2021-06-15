#pragma once

#include "ModernEngine/Window.h"
#include "ModernEngine/Renderer/GraphicsContext.h"
#include "GLFW/glfw3.h"

namespace ModernEngine {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		void OnUpdate() override;
		void SetEventCallback(const EventCallbackFunc& CallbackFunc) override { m_Data.EventCallback = CallbackFunc; }

		void SetVsync(bool enabled) override;
		bool IsVsync() const override;

		inline virtual void* GetNativeWindow() const override { return m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();

	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData
		{
			unsigned int Height, Width;
			std::string Title;
			bool VsyncEnabled;
			EventCallbackFunc EventCallback;
		};

		WindowData m_Data;
	};
}

