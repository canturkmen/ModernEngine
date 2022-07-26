#include "mnpch.h"
#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"

#include "ModernEngine/Scripting/ScripGlue.h"

namespace ModernEngine {

	namespace Utils {

		static char* ReadFileBytes(const std::filesystem::path& filePath, uint32_t* outSize)
		{
			std::ifstream inputFile(filePath, std::ios::binary | std::ios::ate);
			if (!inputFile)
				return nullptr;

			std::streampos end = inputFile.tellg();
			inputFile.seekg(0, std::ios::beg);
			uint64_t size = end - inputFile.tellg();

			if (size == 0)
				return nullptr;

			char* buffer = new char[size];
			inputFile.read((char*)buffer, size);
			inputFile.close();

			*outSize = (uint32_t)size;
			return buffer;
		}

		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = ReadFileBytes(assemblyPath, &fileSize);

			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				MN_CORE_ERROR(errorMessage);

				return nullptr;
			}

			std::string strAssemblyPath = assemblyPath.string(); 

			MonoAssembly* assembly = mono_assembly_load_from_full(image, strAssemblyPath.c_str(), &status, 0);
			mono_image_close(image);

			delete[] fileData;
			return assembly;
		}

		void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				MN_CORE_TRACE("{}.{}", nameSpace, name);
			}
		}
	}

	struct ScriptEngineData 
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoImage* CoreAssemblyImage = nullptr;
		MonoAssembly* CoreAssembly = nullptr;

		ScriptClass EntityClass;

		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
	};

	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData;

		InitMono();
		LoadAssembly("Resources/Scripts/ModernEngine-ScriptCore.dll");
		LoadAssemblyClasses(s_Data->CoreAssembly);
		
		ScripGlue::RegisterFunctions();

#if 0
		s_Data->EntityClass = ScriptClass("ModernEngine", "Entity");

		MonoObject* instance = s_Data->EntityClass.Instantiate();

		MonoMethod* printMessageFunc = s_Data->EntityClass.GetMethod("PrintMessage", 0);
		mono_runtime_invoke(printMessageFunc, instance, nullptr, nullptr);

		MonoMethod* printParamIntFunc = s_Data->EntityClass.GetMethod("PrintInt", 1);

		int value = 10;
		void* param = &value;
		s_Data->EntityClass.InvokeMethod(printParamIntFunc, instance, &param);

		MonoMethod* printParamIntsFunc = s_Data->EntityClass.GetMethod("PrintInts", 2);

		int value2 = 20;
		void* params[2] = {
			&value,
			&value2
		};

		s_Data->EntityClass.InvokeMethod(printParamIntsFunc, instance, params);

		MonoMethod* printParamMessageFunc = s_Data->EntityClass.GetMethod("PrintMessage", 1);

		MonoString* monoString = mono_string_new(s_Data->AppDomain, "Hello World ");
		void* stringParam = monoString;

		s_Data->EntityClass.InvokeMethod(printParamMessageFunc, instance, &stringParam);
#endif
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_Data;
	}

	void ScriptEngine::LoadAssemblyClasses(MonoAssembly* assembly)
	{
		s_Data->EntityClasses.clear();

		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		MonoClass* entityClass = mono_class_from_name(image, "ModernEngine", "Entity");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, name);
			else
				fullName = name;
				
			MonoClass* monoClass = mono_class_from_name(image, nameSpace, name);
			if(monoClass == entityClass ) continue;

			bool isSubclass = mono_class_is_subclass_of(monoClass, entityClass, false);
			if (isSubclass)
				s_Data->EntityClasses[fullName] = CreateRef<ScriptClass>(nameSpace, name);
		}
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib/4.5");

		MonoDomain* rootDomain = mono_jit_init("ModernEngineJITRuntime");
		s_Data->RootDomain = rootDomain;
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filePath)
	{
		MonoDomain* appDomain = mono_domain_create_appdomain("ModernEngineRuntime", nullptr);
		s_Data->AppDomain = appDomain;
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = Utils::LoadMonoAssembly(filePath);
		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		// Utils::PrintAssemblyTypes(s_Data->CoreAssembly);
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullClassName)
	{
		return s_Data->EntityClasses.find(fullClassName) != s_Data->EntityClasses.end();
	}

	std::unordered_map<std::string, Ref<ScriptClass>> ScriptEngine::GetClassEntities()
	{
		return s_Data->EntityClasses;
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);

		return instance;
	}

	void ScriptEngine::ShutdownMono()
	{
		s_Data->AppDomain = nullptr;
		s_Data->RootDomain = nullptr;
	}

	ScriptClass::ScriptClass(const std::string& nameSpace, const std::string& name)
		:m_ClassNameSpace(nameSpace), m_ClassName(name)
	{
		m_MonoClass = mono_class_from_name(s_Data->CoreAssemblyImage, nameSpace.c_str(), name.c_str());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount)
	{
		return mono_class_get_method_from_name(m_MonoClass, name.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoMethod* method, MonoObject* instance, void** params)
	{	
		return mono_runtime_invoke(method, instance, params, nullptr);
	}

}