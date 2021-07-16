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

		void OnUpdate(DeltaTime dt);

	private:
		entt::registry m_Registery;
		friend class Entity;
	};
}

