#include "mnpch.h"
#include "Application.h"
#include "Log.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace ModernEngine {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowCloseEvent CloseEvent;
		KeyPressedEvent PressedEvent(67, 2);
		MouseMovedEvent MovedEvent(20, 40);

		if (CloseEvent.IsInCategory(EventCategoryApplication) || CloseEvent.IsInCategory(EventCategoryInput))
			MN_CORE_TRACE(CloseEvent);

		if (PressedEvent.IsInCategory(EventCategoryKeyboard) || PressedEvent.IsInCategory(EventCategoryInput))
			MN_CORE_INFO(PressedEvent);

		if (MovedEvent.IsInCategory(EventCategoryKeyboard))
			MN_CORE_TRACE(MovedEvent);
		else
			MN_CORE_ERROR("Wrong event category !");

		while (true);
	}

}
