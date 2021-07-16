#pragma once
#include "entt.hpp"
#include "ModernEngine/Core/DeltaTime.h"

namespace ModernEngine {

	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();
		entt::registry& Registery() { return m_Registery; }

		void OnUpdate(DeltaTime dt);

	private:
		entt::registry m_Registery;
	};
}

