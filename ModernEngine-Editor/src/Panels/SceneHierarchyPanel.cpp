#include "SceneHierarchyPanel.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

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

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Entity"))
				m_Context->CreateEntity("Empty Entity");
			
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");
			
			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Camera Component"))
				{
					m_SelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite Renderer Component"))
				{
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}			
		
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
			m_SelectionContext = entity;

		bool EntityDestroyed = false;
		if (ImGui::BeginPopupContextItem())
		{
			if(ImGui::MenuItem("Destroy Entity"))
				EntityDestroyed = true;

			ImGui::EndPopup();
		}

		if (opened)
			ImGui::TreePop();
		
		if (EntityDestroyed)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	static void DrawVec3Component(const std::string& name, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(name.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(name.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 ButtonSize = { lineHeight + 3.0f, lineHeight };


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.3f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.4f, 0.5f, 0.5f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.8f, 0.3f, 1.0f });

		if (ImGui::Button("X", ButtonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.4f, 0.5f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.3f, 0.3f, 1.0f });

		if (ImGui::Button("Y", ButtonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.3f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.4f, 0.6f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.5f, 0.3f, 1.0f });

		if (ImGui::Button("Z", ButtonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
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

		const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.HasComponent<TransformComponent>())
		{
			bool opened = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), flags, "Transform");

			if (opened)
			{
				auto& transform = entity.GetComponent<TransformComponent>();

				DrawVec3Component("Position", transform.Translation);
				glm::vec3 rotation = glm::degrees(transform.Rotation);
				DrawVec3Component("Rotation", rotation);
				transform.Rotation = glm::radians(rotation);
				DrawVec3Component("Scale", transform.Scale);

				ImGui::TreePop();
			}
		} 

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), flags, "Camera"))
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

		if(entity.HasComponent<SpriteRendererComponent>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });

			bool opened = ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), flags, "Sprite Renderer");
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);

			if (ImGui::Button("+", ImVec2{ 20, 20 }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			ImGui::PopStyleVar();

			bool ComponentDeleted = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Sprite Renderer Component"))
					ComponentDeleted = true;

				ImGui::EndPopup();
			}

			if (opened)
			{
				auto& color = entity.GetComponent<SpriteRendererComponent>().Color;
				ImGui::ColorEdit4("Color", glm::value_ptr(color));
				ImGui::TreePop();
			}

			if (ComponentDeleted)
				m_SelectionContext.RemoveComponent<SpriteRendererComponent>();
		}
	}
}