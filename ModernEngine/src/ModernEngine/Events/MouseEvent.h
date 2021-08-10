#pragma once

#include "Event.h"
#include "ModernEngine/Core/MouseButtonCodes.h"
#include <sstream>

namespace ModernEngine {

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float posX, float posY)
			: m_MousePosX(posX), m_MousePosY(posY) {}

		inline float GetMouseXPosition() const { return m_MousePosX; }
		inline float GetMouseYPostition() const { return m_MousePosY; }

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

			std::string Debug() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MousePosX << ", " << m_MousePosY;
			return ss.str();
		}

	private:
		float m_MousePosX, m_MousePosY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xoffset, float yoffset)
			:m_XOffset(xoffset), m_YOffset(yoffset) {}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

			std::string Debug() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

	private:
		float m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline MouseCode GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput | EventCategoryMouse)

	protected:
		MouseButtonEvent(MouseCode button)
			: m_Button(button) {}

		MouseCode m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(MouseCode button)
			:MouseButtonEvent(button) {}


		std::string Debug() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressed:  " << m_Button;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(MouseCode button)
			:MouseButtonEvent(button) {}

		EVENT_CLASS_TYPE(MouseButtonReleased)

		std::string Debug() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleased:  " << m_Button;
			return ss.str();
		}
	};
}
