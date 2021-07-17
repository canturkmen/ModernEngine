#include "mnpch.h"
#include "Scene.h"
#include "Components.h"
#include "ModernEngine/Renderer/Renderer2D.h"
#include "Entity.h"
#include <glm/glm.hpp>

namespace ModernEngine {

	Scene::Scene()
	{
#if 0
		struct MeshComponent 
		{
			float value;
			MeshComponent() = default;
		};

		struct TransformComponent
		{
			glm::mat4 Transform;

			TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;
			TransformComponent(const glm::mat4& transform)
				:Transform(transform) {}

			operator glm::mat4() { return Transform; }
			operator const glm::mat4& () const { return Transform; }
		};

		entt::entity entity = m_Registery.create();
		m_Registery.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		auto view = m_Registery.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = m_Registery.get<TransformComponent>(entity);
		}

		auto group = m_Registery.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		}
#endif
	}

	Scene::~Scene()
	{
		
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registery.create(), this }; 
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::OnUpdate(DeltaTime dt)
	{
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto cameraGroup = m_Registery.view<TransformComponent, CameraComponent>();
			for (auto entity : cameraGroup)
			{
				auto& [transform, camera] = cameraGroup.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.m_Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (mainCamera != nullptr)
		{
			Renderer2D::BeginScene(mainCamera->GetProjectionMatrix(), *cameraTransform);
			auto group = m_Registery.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform, sprite.Color);
			}
		}
		Renderer2D::EndScene();
	}
}
