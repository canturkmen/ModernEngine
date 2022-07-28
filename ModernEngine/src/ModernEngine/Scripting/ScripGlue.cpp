#include "mnpch.h"
#include "ScripGlue.h"

#include "ModernEngine/Scripting/ScriptEngine.h"
#include "ModernEngine/Scene/Scene.h"
#include "ModernEngine/Scene/Entity.h"

#include "ModernEngine/Core/Input.h"	

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

	static void Entity_GetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityWithUUID(entityID);
		*translation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void Entity_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityWithUUID(entityID);
		entity.GetComponent<TransformComponent>().Translation = *translation; 
	}

	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	void ScripGlue::RegisterFunctions()
	{
		MN_ADD_INTERNAL_CALL(NativeLog);
		MN_ADD_INTERNAL_CALL(NativeLog_Vector);
		MN_ADD_INTERNAL_CALL(NativeLog_VectorDot);

		MN_ADD_INTERNAL_CALL(Entity_GetTranslation);
		MN_ADD_INTERNAL_CALL(Entity_SetTranslation);

		MN_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}

}