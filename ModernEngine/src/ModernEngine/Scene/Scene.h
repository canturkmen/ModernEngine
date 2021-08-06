#pragma once
#include "entt.hpp"
#include "ModernEngine/Core/DeltaTime.h"

namespace ModernEngine {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name);
		entt::registry& Registery() { return m_Registery; }

		void DestroyEntity(Entity entity);

		void OnUpdate(DeltaTime dt);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCamera();

	private: 
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registery;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}

