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
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		uint32_t m_Width;
		uint32_t m_Height;

		entt::registry m_Registery;
		friend class Entity;
	};
}

