#include "mnpch.h"
#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

namespace ModernEngine {
	
	struct ScriptEngineData
	{
		MonoDomain* monoDomain;
		MonoDomain* appDomain;
	};

	static ScriptEngineData* s_Data;

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();

		InitMono();
	}
	
	void ScriptEngine::ShutDown()
	{
		delete s_Data;

		ShutDownMono();
	}
	
	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("ModernEngineRuntime");

		s_Data->monoDomain = rootDomain;
		s_Data->appDomain = mono_domain_create_appdomain("ModernEngineScriptRuntime", nullptr);

	}

	void ScriptEngine::ShutDownMono()
	{

	}
}

