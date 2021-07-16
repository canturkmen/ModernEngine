#include "mnpch.h"
#include "Entity.h"

namespace ModernEngine {

	Entity::Entity(entt::entity entityhandle, Scene* scene)
		: m_EntityHandle(entityhandle), m_Scene(scene)
	{

	}

}

