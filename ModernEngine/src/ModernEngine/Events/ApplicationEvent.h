#pragma once

#include "Event.h"

namespace ModernEngine {

	class MN_API WindowResizeEvent : public Event
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

	class MN_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryApplication)
	};

	class MN_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryApplication)
	};

	class MN_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryApplication)
	};

	class MN_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryApplication)
	};

}
