#pragma once

#include <filesystem>

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
}

namespace ModernEngine {

	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void LoadAssembly(const std::filesystem::path& filePath);
	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses(MonoAssembly* assembly);

		friend class ScriptClass;
	};

	class ScriptClass 
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& nameSpace, const std::string& name);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* InvokeMethod(MonoMethod* method, MonoObject* instance, void** params);

	private:
		std::string m_ClassNameSpace;
		std::string m_ClassName;

		MonoClass* m_MonoClass = nullptr;
	};
}

