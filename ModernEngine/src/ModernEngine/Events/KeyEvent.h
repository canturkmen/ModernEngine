#pragma once

#include "Event.h"
#include "ModernEngine/Core/KeyCodes.h"

namespace ModernEngine {

	class KeyEvent : public Event
	{
	public:
		inline KeyCode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(KeyCode keycode)
			: m_KeyCode(keycode) {}

		KeyCode m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keycode, int repeatCount)
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

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keycode)
			: KeyEvent(keycode) {}

		EVENT_CLASS_TYPE(KeyReleased)

		std::string Debug() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(KeyCode keycode)
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
