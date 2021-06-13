#pragma once

#include "Event.h"

namespace ModernEngine {

	class MN_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class MN_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		EVENT_CLASS_TYPE(KeyPressed)

		std::string Debug() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << ", Repeat Count: " << m_RepeatCount;
			return ss.str();
		}

	private:
		int m_RepeatCount;
	};

	class MN_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		EVENT_CLASS_TYPE(KeyReleased)

		std::string Debug() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}
	};

	class MN_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		EVENT_CLASS_TYPE(KeyTyped)

		std::string Debug() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}
	};
}
