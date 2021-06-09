#pragma once

#include "Core.h"

namespace ModernEngine {

	class MN_API Application
	{
	public:
		Application();
		virtual ~Application();
	
		void Run();
	};

	Application* CreateApplication();

}

