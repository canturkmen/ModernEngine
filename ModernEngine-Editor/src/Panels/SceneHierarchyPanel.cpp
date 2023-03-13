#include "SceneHierarchyPanel.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <cstring>
#include <filesystem>

#include "./ModernEngine/Scripting/ScriptEngine.h"

// To avoid a compiler warning when the strncpy is used
#ifdef _MSVC_LANG
	#define _CRT_SECURE_NO_WARNINGS
#endif

namespace ModernEngine {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		if (m_Context)
		{
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

		}

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
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

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
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

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
		ImGui::PushFont(boldFont);

		if (ImGui::Button("X", ButtonSize))
			values.x = resetValue;

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.4f, 0.5f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.3f, 0.3f, 1.0f });
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Y", ButtonSize))
			values.y = resetValue;

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.3f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.4f, 0.6f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.5f, 0.3f, 1.0f });
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Z", ButtonSize))
			values.x = resetValue;

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction function)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2& RegionAvailable = ImGui::GetContentRegionAvail();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(RegionAvailable.x - lineHeight * 0.5f);

			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool ComponentDeleted = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					ComponentDeleted = true;

				ImGui::EndPopup();
			}

			if (opened)
			{
				function(component);
				ImGui::TreePop();
			}

			if (ComponentDeleted)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<ScriptComponent>("Script");
			DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
			DisplayAddComponentEntry<CircleRendererComponent>("Circle Renderer");
			DisplayAddComponentEntry<Rigidbody2DComponent>("Rigidbody 2D");
			DisplayAddComponentEntry<BoxCollider2DComponent>("Box Collider 2D");
			DisplayAddComponentEntry<CircleCollider2DComponent>("Circle Collider 2D");
			DisplayAddComponentEntry<TextComponent>("Text Component");

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			DrawVec3Component("Position", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Component("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Component("Scale", component.Scale);
		});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.m_Camera;
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
				float PerspectiveFOV = glm::degrees(camera.GetPerspectiveCameraFOV());
				if (ImGui::DragFloat("Vertical FOV", &PerspectiveFOV))
					camera.SetPerspectiveCameraFOV(glm::radians(PerspectiveFOV));

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
		});

		DrawComponent<ScriptComponent>("Script", entity, [entity, scene = m_Context](auto& component) mutable
		{ 
			bool classExists = ScriptEngine::EntityClassExists(component.ClassName);

			static char buffer[64];
			strncpy_s(buffer, sizeof(buffer), component.ClassName.c_str(), sizeof(buffer));

			if (!classExists)
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.9f, 0.1f, 0.1f, 1.0f });

			if (ImGui::InputText("Class", buffer, sizeof(buffer)))
				component.ClassName = buffer;

			bool running = scene->GetIsRunning();
			if (running)
			{
				// Fields
				Ref<ScriptInstance> scriptInstance = ScriptEngine::GetEntityScriptInstance(entity.GetUUID());
				if (scriptInstance)
				{
					const auto& fields = scriptInstance->GetScriptClass()->GetFields();
					for (const auto& [name, field] : fields)
					{
						if (field.Type == ScriptClassFieldType::Float)
						{
							float data = scriptInstance->GetValue<float>(name); 
							if (ImGui::DragFloat(name.c_str(), &data))
								scriptInstance->SetValue(name, data);
						}
					}
				}
			}
			else
			{
				if (classExists)
				{
					Ref<ScriptClass> entityClass = ScriptEngine::GetEntityScriptClass(component.ClassName);
					const auto& entityFields = entityClass->GetFields();
					auto& fieldInstanceMap = ScriptEngine::GetScriptFieldMap(entity);
					for (const auto& [name, field] : entityFields)
					{
						if (fieldInstanceMap.find(name) != fieldInstanceMap.end())
						{
							ScriptFieldInstance& scriptField = fieldInstanceMap.at(name);

							if (field.Type == ScriptClassFieldType::Float)
							{
								float data = scriptField.GetValue<float>(); 
								if (ImGui::DragFloat(name.c_str(), &data))
									scriptField.SetValue(data);
							}
						}
						else
						{
							if (field.Type == ScriptClassFieldType::Float)
							{
								float data = 0.0f;
								if (ImGui::DragFloat(name.c_str(), &data))
								{
									ScriptFieldInstance& scriptField = fieldInstanceMap[name];
									scriptField.Field = field;
									scriptField.SetValue(data);
								}
							}
						}
					}
				}
			}

			if (!classExists)
				ImGui::PopStyleColor();
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component) 
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			ImGui::Button("Texture", ImVec2{ 100.0f, 0.0f });
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content Browser Item"))
				{
					const wchar_t* data = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath(data);
					component.Texture = Texture2D::Create(texturePath.string());
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
		});

		DrawComponent<CircleRendererComponent>("Circle Renderer", entity, [](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::DragFloat("Thickness", &component.Thickness, 0.1f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &component.Fade, 0.1f, 0.0f, 1.0f);
			});

		DrawComponent<Rigidbody2DComponent>("Rigidbody 2D Component", entity, [](auto& component)
			{
				const char* BodyTypeName[] = { "Static", "Dynamic", "Kinematic"};
				const char* CurrentBodyTypeName = BodyTypeName[(int)component.BType];

				if (ImGui::BeginCombo("Body Type", CurrentBodyTypeName))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = CurrentBodyTypeName == BodyTypeName[i];

						if (ImGui::Selectable(BodyTypeName[i], isSelected))
						{
							CurrentBodyTypeName = BodyTypeName[i];
							component.BType = (Rigidbody2DComponent::BodyType)i;
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::Checkbox("Fixed Rotation", &component.fixedRotation);
			});


		DrawComponent<BoxCollider2DComponent>("Box Collider 2D Component", entity, [](auto& component)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));
				ImGui::DragFloat2("Size", glm::value_ptr(component.size));
				ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("RestitutionThreshold", &component.RestitutionThreshold, 0.01f, 0.0f);
			});

		DrawComponent<CircleCollider2DComponent>("Circle Collider 2D Component", entity, [](auto& component)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));
				ImGui::DragFloat("Radius",  &component.Radius);
				ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("RestitutionThreshold", &component.RestitutionThreshold, 0.01f, 0.0f);
			});

		DrawComponent<TextComponent>("Text Renderer", entity, [](auto& component)
			{
				ImGui::InputTextMultiline("Text String", &component.TextString);
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::DragFloat("Kerning", &component.Kerning, 0.025f);
				ImGui::DragFloat("Line Spacing", &component.LineSpacing, 0.025f);
			});
	}

	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName)
	{
		if (!m_SelectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}
}