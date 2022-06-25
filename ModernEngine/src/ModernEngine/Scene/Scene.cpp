#include "mnpch.h"
#include "Scene.h"
#include "Components.h"

#include "ModernEngine/Renderer/Renderer2D.h"
#include "ModernEngine/Scene/ScriptableEntity.h"
#include "Entity.h"
#include <glm/glm.hpp>

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"
#include "box2d/b2_fixture.h"

namespace ModernEngine {

	static b2BodyType Rigidbody2DComponentTypeToBox2DType(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case Rigidbody2DComponent::BodyType::Static:  return b2BodyType::b2_staticBody;
			case Rigidbody2DComponent::BodyType::Dynamic:  return b2BodyType::b2_dynamicBody;
			case Rigidbody2DComponent::BodyType::Kinematic:  return b2BodyType::b2_kinematicBody;
		}

		return b2BodyType::b2_staticBody;
	}

	Scene::Scene()
	{
	
	}

	Scene::~Scene()
	{
		delete m_ActivePhysicsWorld;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
			{
				auto view = src.view<Component>();
				for (auto e : view)
				{
					UUID id = src.get<IDComponent>(e).uuid;
					MN_CORE_ASSERT(enttMap.find(id) != enttMap.end(), "Key value could not be found !")
					entt::entity dstEnttId = enttMap.at(id);

					auto& component = src.get<Component>(e);
					dst.emplace_or_replace<Component>(dstEnttId, component);
				}
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component ...>,  entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponent<Component ...>(dst, src, enttMap);
	}
	
	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		auto& srcSceneRegistery = other->m_Registery;
		auto& dstSceneRegistery = newScene->m_Registery;
		std::unordered_map<UUID, entt::entity> enttMap;

		auto idView = srcSceneRegistery.view<IDComponent>();

		// Copy the entities with their UUID.
		for (auto e : idView)
		{
			UUID uuid = srcSceneRegistery.get<IDComponent>(e).uuid;
			const auto& name = srcSceneRegistery.get<TagComponent>(e).Tag;
			Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = newEntity;
		}

		// Copy the components into the new scene expect ID and Tag component.
		CopyComponent(AllComponents{}, dstSceneRegistery, srcSceneRegistery, enttMap);

		return newScene;
	}

	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]() 
		{
			if (src.HasComponent<Component>())
				dst.AddComponentOrReplace<Component>(src.GetComponent<Component>());
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component ...>,  Entity dst, Entity src)
	{
		CopyComponentIfExists<Component ...>(dst, src);
	}

	void Scene::DuplicateEntity(Entity entity)
	{
		Entity newEntity = CreateEntity(entity.GetName());
		CopyComponentIfExists(AllComponents{}, newEntity, entity);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registery.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registery.destroy(entity);
	}

	void Scene::OnPhysics2DStart()
	{
		m_ActivePhysicsWorld = new b2World({ 0.0f, -9.8f });
		auto view = m_Registery.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& r2bd = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = Rigidbody2DComponentTypeToBox2DType(r2bd.BType);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;

			b2Body* body = m_ActivePhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(r2bd.fixedRotation);
			r2bd.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape shape;
				shape.SetAsBox(bc2d.size.x * transform.Scale.x, bc2d.size.y * transform.Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &shape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape CircleShape;
				CircleShape.m_p.Set(cc2d.offset.x, cc2d.offset.y);
				CircleShape.m_radius = transform.Scale.x * cc2d.Radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &CircleShape;
				fixtureDef.density = cc2d.Density;
				fixtureDef.friction = cc2d.Friction;
				fixtureDef.restitution = cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnPhysics2DStop()
	{
		delete m_ActivePhysicsWorld;
		m_ActivePhysicsWorld = nullptr;
	}

	void Scene::OnStartRuntime()
	{
		OnPhysics2DStart();
	}

	void Scene::OnStopRuntime()
	{
		OnPhysics2DStop();
	}

	void Scene::OnSimulationStart()
	{
		OnPhysics2DStart();
	}

	void Scene::OnSimulationStop()
	{
		OnPhysics2DStop();
	}

	void Scene::OnUpdateEditor(DeltaTime dt, EditorCamera& camera)
	{
		RenderScene(camera);
	}

	void Scene::OnUpdateRuntime(DeltaTime dt)
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

		// Physics 
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 6;
			m_ActivePhysicsWorld->Step(dt, velocityIterations, positionIterations);

			auto view = m_Registery.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

				b2Body* body = (b2Body*)rb2d.RuntimeBody;
				const auto& position = body->GetPosition();
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z = body->GetAngle();
			}
		} 

		// 2D render
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto cameraGroup = m_Registery.view<TransformComponent, CameraComponent>();
			for (auto entity : cameraGroup)
			{
				auto [transform, camera] = cameraGroup.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.m_Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera != nullptr)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);
			{
				auto group = m_Registery.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group)
				{
					auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
					Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
				}
			}
			
			{
				auto view = m_Registery.view<TransformComponent, CircleRendererComponent>();
				for (auto entity : view)
				{
					auto& [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);
					Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
				}
			}
		}

		Renderer2D::EndScene();
	}

	void Scene::OnSimulationUpdate(DeltaTime dt, EditorCamera& camera)
	{
		// Physics 
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 6;
			m_ActivePhysicsWorld->Step(dt, velocityIterations, positionIterations);

			auto view = m_Registery.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

				b2Body* body = (b2Body*)rb2d.RuntimeBody;
				const auto& position = body->GetPosition();
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z = body->GetAngle();
			}
		}

		RenderScene(camera);
	}

	void Scene::RenderScene(EditorCamera& camera)
	{

		// Render the editor scene
		Renderer2D::BeginScene(camera);
		{
			auto group = m_Registery.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}
		}

		{
			auto view = m_Registery.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : view)
			{
				auto& [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);
				Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
			}
		}

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registery.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.m_Camera.SetViewportSize(width, height);
		}
	}

	Entity Scene::GetPrimaryCamera()
	{
		auto view = m_Registery.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (cameraComponent.Primary)
				return Entity{ entity, this };
		}

		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
		
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.m_Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
	{

	}


	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
		
	}

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{

	}
}
