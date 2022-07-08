#pragma once

#include "mnpch.h"

#include "Base.h"
#include "ModernEngine/Events/Event.h"

namespace ModernEngine {

	struct WindowProps
	{
		uint32_t Height;
		uint32_t Width;
		std::string Title;

		WindowProps(const std::string& title = "Modern Engine", uint32_t height = 900, uint32_t width = 1600)
			: Title(title), Height(height), Width(width) {}
	};

	// This class is an interface for the sub window classes such as WindowsWindow, LinuxWindow and MacWindow
	class Window
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
		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};

}
