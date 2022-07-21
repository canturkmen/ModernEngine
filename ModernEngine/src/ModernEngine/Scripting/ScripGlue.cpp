#include "mnpch.h"
#include "ScripGlue.h"

#include "mono/metadata/object.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>

namespace ModernEngine {

#define MN_ADD_INTERNAL_CALL(name) mono_add_internal_call("ModernEngine.InternalCalls::" #name, name)
 
	static void NativeLog(MonoString* string, int value)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str = cStr;
		mono_free(cStr);

		std::cout << str << ", " << value << std::endl;
	}

	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
	{
		*outResult = glm::normalize(*parameter);
	}

	static float NativeLog_VectorDot(glm::vec3* parameter)
	{
		return glm::dot(*parameter, *parameter);
	}

	void ScripGlue::RegisterFunctions()
	{
		MN_ADD_INTERNAL_CALL(NativeLog);
		MN_ADD_INTERNAL_CALL(NativeLog_Vector);
		MN_ADD_INTERNAL_CALL(NativeLog_VectorDot);
	}

}