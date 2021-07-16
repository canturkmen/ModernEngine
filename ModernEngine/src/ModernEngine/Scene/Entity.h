#pragma once

#include "Scene.h"

namespace ModernEngine {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entityhandle, Scene* scene);

		template<typename T, typename ... Args>
		T& AddComponent(Args&&... args)
		{
			return m_Scene->m_Registery.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->m_Registery.get<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_Scene->m_Registery.remove<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registery.all_of<T>(m_EntityHandle);
		}

	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	};

}