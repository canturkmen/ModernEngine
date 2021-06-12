#pragma once

#include "Event.h"

namespace ModernEngine {

	class MN_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput)

	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {}

		int m_Button;
	};

	class MN_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			:MouseButtonEvent(button) {}

		EVENT_CLASS_TYPE(MouseButtonPressed)

		std::string Debug() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressed:  " << m_Button;
			return ss.str();
		}
	};

	class MN_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			:MouseButtonEvent(button) {}

		EVENT_CLASS_TYPE(MouseButtonReleased)

		std::string Debug() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleased:  " << m_Button;
			return ss.str();
		}
	};

	class MN_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float posX, float posY)
			: m_MousePosX(posX), m_MousePosY(posY) {}

		inline float GetMouseXPosition() const { return m_MousePosX; }
		inline float GetMouseYPostition() const { return m_MousePosY; }

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

		std::string Debug() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MousePosX << ", " << m_MousePosY;
			return ss.str();
		}

	private:
		float m_MousePosX, m_MousePosY;
	};

	class MN_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xoffset, float yoffset)
			:m_XOffset(xoffset), m_YOffset(yoffset) {}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

		std::string Debug() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

	private:
		float m_XOffset, m_YOffset;
	};
}
