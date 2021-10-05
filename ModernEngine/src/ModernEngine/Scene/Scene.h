#pragma once
#include "entt.hpp"
#include "ModernEngine/Core/DeltaTime.h"
#include "ModernEngine/Core/UUID.h"
#include "ModernEngine/Renderer/EditorCamera.h"
#include "ModernEngine/Renderer/Texture.h"

class b2World;

namespace ModernEngine {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();	

		static Ref<Scene> Copy(Ref<Scene> other);
		void DuplicateEntity(Entity entity);

		Entity CreateEntity(const std::string& name);
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name);
		entt::registry& Registery() { return m_Registery; }

		void DestroyEntity(Entity entity);

		void OnStartRuntime();
		void OnStopRuntime();

		void OnUpdateEditor(DeltaTime dt, EditorCamera& camera);
		void OnUpdateRuntime(DeltaTime dt);

		void OnViewportResize(uint32_t width, uint32_t height);


		Entity GetPrimaryCamera();

	private: 
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registery;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2World* m_ActivePhysicsWorld = nullptr;
		
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}

