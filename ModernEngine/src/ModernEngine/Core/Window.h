#pragma once

#include "mnpch.h"

#include "Core.h"
#include "ModernEngine/Events/Event.h"

namespace ModernEngine {

	struct WindowProps
	{
		unsigned int Height;
		unsigned int Width;
		std::string Title;

		WindowProps(unsigned int height = 720, unsigned int width = 1280, const std::string& title = "Modern Engine")
			: Height(height), Width(width), Title(title) {}
	};


	// This class is an interface for the sub window classes such as WindowsWindow, LinuxWindow and MacWindow
	class MN_API Window
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		virtual ~Window() = default;
		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual void SetVsync(bool enabled) = 0;
		virtual bool IsVsync() const = 0;
		virtual void SetEventCallback(const EventCallbackFunc& CallbackFunc) = 0;

		virtual void* GetNativeWindow() const = 0;
		static Window* Create(const WindowProps& props = WindowProps());
	};

}
