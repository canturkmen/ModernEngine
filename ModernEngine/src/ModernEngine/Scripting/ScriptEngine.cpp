#include "mnpch.h"
#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"

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

	char* ReadFileBytes(const std::string& filePath, uint32_t* outSize)
	{
		std::ifstream inputFile(filePath, std::ios::binary | std::ios::ate);
		if (!inputFile)
			return nullptr;

		std::streampos end = inputFile.tellg();
		inputFile.seekg(0, std::ios::beg);
		uint32_t size = end - inputFile.tellg();

		if (size == 0)
			return nullptr;

		char* buffer = new char[size];
		inputFile.read((char*)buffer, size);
		inputFile.close();

		*outSize = size;
		return buffer;
	}

	MonoAssembly* LoadCAssembly(const std::string& assemblyPath)
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

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		delete[] fileData; 
		return assembly;
	}

	void PrintAssemblyTypes(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for(int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			MN_CORE_TRACE("{}.{}", nameSpace, name);
		}
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib/4.5");

		MonoDomain* rootDomain = mono_jit_init("ModernEngineJITRuntime");
		s_Data->RootDomain = rootDomain;

		MonoDomain* appDomain = mono_domain_create_appdomain("ModernEngineRuntime", nullptr);
		s_Data->AppDomain = appDomain;
		mono_domain_set(s_Data->AppDomain, true);
		
		s_Data->CoreAssembly = LoadCAssembly("Resources/Scripts/ModernEngine-ScriptCore.dll");
		PrintAssemblyTypes(s_Data->CoreAssembly);
	}

	void ScriptEngine::ShutdownMono()
	{
		s_Data->AppDomain = nullptr;
		s_Data->RootDomain = nullptr;
	}

}