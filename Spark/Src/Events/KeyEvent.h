#pragma once

#include <sstream>

#include "Events/Event.h"

namespace Spark
{
	class KeyEvent  : public Event
	{
	public:
		int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput);


	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}
		int m_KeyCode;
	};

	class KeyPressedEvent final : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode)
			, m_RepeatCount(repeatCount)
		{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		inline int GetRepeatcount() const { return m_RepeatCount; }

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;

	};

	class KeyReleasedEvent final : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode)
		{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)

	};
}
