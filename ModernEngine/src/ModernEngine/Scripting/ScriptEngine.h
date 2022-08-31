#pragma once

#include <filesystem>

#include "ModernEngine/Scene/Scene.h"
#include "ModernEngine/Scene/Entity.h"

#include <map>;

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
}

namespace ModernEngine {

	enum class ScriptClassFieldType
	{
		None = 0,
		Float, Double,
		Bool, Char, Byte, Short, Int, Long,
		UByte, UShort, UInt, ULong,
		Vector2, Vector3, Vector4,
		Entity
	};

	struct ScriptField
	{
		ScriptClassFieldType Type;
		std::string Name;
		MonoClassField* ClassField;
	};	

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& nameSpace, const std::string& name, bool isCore = false);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* InvokeMethod(MonoMethod* method, MonoObject* instance, void** params = nullptr);

		const std::map<std::string, ScriptField>& GetFields() { return m_Fields; }

	private:
		std::string m_ClassNameSpace;
		std::string m_ClassName;
			
		std::map<std::string, ScriptField> m_Fields;

		MonoClass* m_MonoClass = nullptr;

		friend class ScriptEngine;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);

		void InvokeOnCreate();
		void InvokeOnUpdate(float dt);

		Ref<ScriptClass> GetScriptClass() { return m_ScriptClass; }

		template<typename T>
		T GetValue(const std::string& name)
		{
			bool success = GetValueInternal(name, FieldValueBuffer);
			if (success)
				T();

			return *(T*)FieldValueBuffer;
		}

		template<typename T>
		void SetValue(const std::string& name, const T& value)
		{
			SetValueInternal(name, &value);	
		}

	private:
		bool GetValueInternal(const std::string& name, void* data);
		bool SetValueInternal(const std::string& name, const void* data);

	private:
		Ref<ScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_EntityConstructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;
		
		inline static char FieldValueBuffer[8];
	};

	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void LoadAssembly(const std::filesystem::path& filePath);
		static void LoadAppAssembly(const std::filesystem::path& filePath);

		static bool EntityClassExists(const std::string& fullClassName);
		static std::unordered_map<std::string, Ref<ScriptClass>> GetClassEntities();

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static void OnCreateEntity(Entity entity)	;
		static void OnUpdateEntity(Entity entity, DeltaTime dt);

		static Scene* GetSceneContext();
		static MonoImage* GetCoreAssemblyImage();
		
		static Ref<ScriptInstance> GetEntityScriptInstance(UUID entityID);
	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses();

		friend class ScriptClass;
		friend class ScriptGlue;
	};
}

