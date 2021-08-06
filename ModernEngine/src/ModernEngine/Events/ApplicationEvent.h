#pragma once

#include "Event.h"

namespace ModernEngine {

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int windowwidth, int windowheight)
			: m_WindowWidth(windowwidth), m_WindowHeight(windowheight) {}

		inline float GetWidth() const { return m_WindowWidth; }
		inline float GetHeight() const { return m_WindowHeight; }

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryApplication)

		std::string Debug() const override
		{
			std::stringstream ss;
			ss << "WindowResizedEvent: " << m_WindowWidth << ", " << m_WindowHeight;
			return ss.str();
		}

	private:
		int m_WindowWidth, m_WindowHeight;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryApplication)
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryApplication)
	};

}
