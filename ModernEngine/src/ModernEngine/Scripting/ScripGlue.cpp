#include "mnpch.h"
#include "ScriptGlue.h"

#include "ModernEngine/Scripting/ScriptEngine.h"
#include "ModernEngine/Scene/Scene.h"
#include "ModernEngine/Scene/Entity.h"

#include "ModernEngine/Core/Input.h"	

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>

#include "box2d/b2_body.h"

namespace ModernEngine {

	static std::unordered_map<MonoType*, std::function<bool(Entity&)>> s_EntityHasComponentFuncs;

#define MN_ADD_INTERNAL_CALL(name) mono_add_internal_call("ModernEngine.InternalCalls::" #name, name)

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* type)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityWithUUID(entityID);

		MonoType* monoComponentType = mono_reflection_type_get_type(type);
		return s_EntityHasComponentFuncs.at(monoComponentType)(entity);
	}
		
	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityWithUUID(entityID);
		*translation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityWithUUID(entityID);
		entity.GetComponent<TransformComponent>().Translation = *translation; 
	}

	static void Rigidbody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityWithUUID(entityID);
		
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityWithUUID(entityID);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}
	
	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	template<typename... Component>
	static void RegisterComponent()
	{
		([]()
		{
			// Extract the managed type name
			std::string_view typeName = typeid(Component).name();
			size_t position = typeName.find_last_of(':');
			std::string_view structName = typeName.substr(position + 1); 
			std::string managedTypeName = fmt::format("ModernEngine.{}", structName);

			MonoType* managedType = mono_reflection_type_from_name(managedTypeName.data(), ScriptEngine::GetCoreAssemblyImage());
			if (!managedType)
			{
				MN_CORE_ERROR("Specified Component Type Does Not Exist !");
				return;
			}
			s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
		}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component ...>)
	{
		RegisterComponent<Component ...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::RegisterFunctions()
	{
		MN_ADD_INTERNAL_CALL(Entity_HasComponent);
		MN_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		MN_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

		MN_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulse);
		MN_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulseToCenter);

		MN_ADD_INTERNAL_CALL(Input_IsKeyDown);

	}
}