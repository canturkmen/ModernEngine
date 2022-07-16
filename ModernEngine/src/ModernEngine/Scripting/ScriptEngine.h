#pragma once

namespace ModernEngine {

	class ScriptEngine
	{
	public:
		static void Init();
		static void ShutDown();

	private:
		static void InitMono();
		static void ShutDownMono();
	};
}


