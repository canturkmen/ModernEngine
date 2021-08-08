#pragma once

#include "ModernEngine/Scene/Scene.h"
#include "ModernEngine/Scene/Entity.h"
#include "ModernEngine/Scene/Components.h"
#include "ModernEngine/Core/Base.h"

namespace ModernEngine {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);
		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}

