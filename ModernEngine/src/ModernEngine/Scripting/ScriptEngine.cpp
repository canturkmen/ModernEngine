#include "mnpch.h"
#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

namespace ModernEngine {

	struct ScriptEngineData 
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;
		MonoAssembly* CoreAssembly = nullptr;
	};

	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData;

		InitMono();
	}

	void ScriptEngine::Shutdown()
	{
		delete s_Data;

		ShutdownMono();
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib/4.5");

		MonoDomain* rootDomain = mono_jit_init("ModernEngineJITRuntime");
		s_Data->RootDomain = rootDomain;

		MonoDomain* appDomain = mono_domain_create_appdomain("ModernEngineRuntime", nullptr);
		s_Data->AppDomain = appDomain;
		mono_domain_set(s_Data->AppDomain, true);


	}

	void ScriptEngine::ShutdownMono()
	{
		s_Data->AppDomain = nullptr;
		s_Data->RootDomain = nullptr;
	}

}