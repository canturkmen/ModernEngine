#include "mnpch.h"
#include "Scene.h"
#include "Components.h"
#include "ModernEngine/Renderer/Renderer2D.h"
#include "Entity.h"
#include <glm/glm.hpp>

namespace ModernEngine {

	Scene::Scene()
	{

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
		// Update Scripts
		{
			m_Registery.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc)
				{
					if (nsc.Instance == nullptr)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(dt);
				});
		}

		// 2D render
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto cameraGroup = m_Registery.view<TransformComponent, CameraComponent>();
			for (auto entity : cameraGroup)
			{
				auto [transform, camera] = cameraGroup.get<TransformComponent, CameraComponent>(entity);

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

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;

		auto view = m_Registery.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.m_Camera.SetViewportSize(width, height);
			
		}
	}
}
