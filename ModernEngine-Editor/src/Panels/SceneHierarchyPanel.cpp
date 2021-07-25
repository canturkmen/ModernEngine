#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace ModernEngine {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registery.each([&](auto entityID)
			{
				Entity entity = { entityID, m_Context.get() };
				DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
			DrawComponents(m_SelectionContext);
		
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
			m_SelectionContext = entity;

		if (opened)
		{
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, tag.c_str());
			if(ImGui::InputText("Tag", buffer, sizeof(buffer))) 
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& camera = entity.GetComponent<CameraComponent>().m_Camera;

				const char* ProjectionTypeName[2] = { "Perspective", "Orthographic" };
				const char* CurrentProjectionTypeName = ProjectionTypeName[(int)camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", CurrentProjectionTypeName))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = CurrentProjectionTypeName == ProjectionTypeName[i];

						if (ImGui::Selectable(ProjectionTypeName[i], isSelected))
						{
							CurrentProjectionTypeName = ProjectionTypeName[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus(); 
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float PerpsectiveFOV = glm::degrees(camera.GetPerspectiveCameraFOV());
					if (ImGui::DragFloat("Vertical FOV", &PerpsectiveFOV))
						camera.SetPerspectiveCameraFOV(glm::radians(PerpsectiveFOV));

					float PerspectiveNear = camera.GetPerspectiveCameraNear();
					if (ImGui::DragFloat("Near Clip", &PerspectiveNear))
						camera.SetPerspectiveCameraNear(PerspectiveNear);

					float PerspectiveFar = camera.GetPerspectiveCameraFar();
					if (ImGui::DragFloat("Far Clip", &PerspectiveFar))
						camera.SetPerspectiveCameraFar(PerspectiveFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float OrthoSize = camera.GetOrthographicCameraSize();
					if (ImGui::DragFloat("Size", &OrthoSize))
						camera.SetOrthographicCameraSize(OrthoSize);

					float OrthoNear = camera.GetOrthographicCameraNear();
					if (ImGui::DragFloat("Near Clip", &OrthoNear))
						camera.SetOrthographicCameraNear(OrthoNear); 

					float OrthoFar = camera.GetOrthographicCameraNear();
					if (ImGui::DragFloat("Far Clip", &OrthoFar))
						camera.SetOrthographicCameraFar(OrthoFar);
				}

				ImGui::TreePop();
			}
		}
	}
}