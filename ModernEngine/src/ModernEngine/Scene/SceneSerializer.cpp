#include "mnpch.h"
#include "SceneSerializer.h"

#include "Components.h"
#include "Entity.h"

#include "ModernEngine/Scripting/ScriptEngine.h"
#include "ModernEngine/Core/UUID.h"
#include "ModernEngine/Project/Project.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<ModernEngine::UUID>
	{
		static Node encode(const ModernEngine::UUID& rhs)
		{
			Node node;
			node.push_back((uint64_t)rhs);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, ModernEngine::UUID& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs = node[0].as<uint64_t>();

			return true;
		}
	};
}

namespace ModernEngine {

#define WRITE_SCRIPT_FIELD(FieldType, Type)					   \
			case ScriptClassFieldType::FieldType:				   \
				 out << scriptFieldInstance.GetValue<Type>();	   \
				 break	

#define READ_SCRIPT_FIELD(FieldType, Type)					   \
			case ScriptClassFieldType::FieldType:				   \
			{													   \
				Type data = scriptField["Data"].as<Type>();		   \
				scriptFieldInstance.SetValue(data);                \
				break;                                             \
			}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		:m_Scene(scene)
	{
	}

	static std::string Rigidbody2DBodyTypeToString(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case Rigidbody2DComponent::BodyType::Static:		return "Static";
		case Rigidbody2DComponent::BodyType::Dynamic:		return "Dynamic";
		case Rigidbody2DComponent::BodyType::Kinematic:		return "Kinematic";
		}

		return {};
	}

	static Rigidbody2DComponent::BodyType StringToRigidbody2DBodyType(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static")			return Rigidbody2DComponent::BodyType::Static;
		if (bodyTypeString == "Dynamic")		return Rigidbody2DComponent::BodyType::Dynamic;
		if (bodyTypeString == "Kinematic")		return Rigidbody2DComponent::BodyType::Kinematic;

		return Rigidbody2DComponent::BodyType::Static;
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag.c_str();

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& transformComponent = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << transformComponent.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << transformComponent.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transformComponent.Scale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.m_Camera;

			out << YAML::Key << "Camera" << YAML::Value;

			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveCameraFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveCameraNear();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveCameraFar();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicCameraSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicCameraNear();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicCameraFar();
			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<ScriptComponent>())
		{
			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap;

			auto& scriptComponent = entity.GetComponent<ScriptComponent>();
			out << YAML::Key << "ClassName" << YAML::Value << scriptComponent.ClassName;

			// Fields
			Ref<ScriptClass> scriptClass = ScriptEngine::GetEntityScriptClass(scriptComponent.ClassName);
			const auto& scriptFields = scriptClass->GetFields();
			if (scriptFields.size() > 0)
			{
				out << YAML::Key << "ScriptFields" << YAML::Value;

				auto& scriptFieldInstanceMap = ScriptEngine::GetScriptFieldMap(entity);
				out << YAML::BeginSeq;
				for (const auto& [name, field] : scriptFields)
				{
					if (scriptFieldInstanceMap.find(name) == scriptFieldInstanceMap.end())
						continue;

					out << YAML::BeginMap;

					out << YAML::Key << "Name" << YAML::Value << name;
					out << YAML::Key << "Type" << YAML::Value << Utils::ScriptClassFieldTypeToString(field.Type);
					out << YAML::Key << "Data" << YAML::Value;

					ScriptFieldInstance& scriptFieldInstance = scriptFieldInstanceMap.at(name);
					switch (field.Type)
					{
						WRITE_SCRIPT_FIELD(Float, float);
						WRITE_SCRIPT_FIELD(Double, double);
						WRITE_SCRIPT_FIELD(Bool, bool);
						WRITE_SCRIPT_FIELD(Char, char);
						WRITE_SCRIPT_FIELD(Byte, int8_t);
						WRITE_SCRIPT_FIELD(Short, int16_t);
						WRITE_SCRIPT_FIELD(Int, int32_t);
						WRITE_SCRIPT_FIELD(Long, int64_t);
						WRITE_SCRIPT_FIELD(UByte, uint8_t);
						WRITE_SCRIPT_FIELD(UShort, uint16_t);
						WRITE_SCRIPT_FIELD(UInt, uint32_t);
						WRITE_SCRIPT_FIELD(ULong, uint64_t);
						WRITE_SCRIPT_FIELD(Vector2, glm::vec2);
						WRITE_SCRIPT_FIELD(Vector3, glm::vec3);
						WRITE_SCRIPT_FIELD(Vector4, glm::vec4);
						WRITE_SCRIPT_FIELD(Entity, UUID);
					}
					out << YAML::EndMap;
				}
				out << YAML::EndSeq;
			}
			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

			if (spriteRendererComponent.Texture)
				out << YAML::Key << "TexturePath" << YAML::Value << spriteRendererComponent.Texture->GetFilePath();
			

			out << YAML::Key << "TilingFactor" << YAML::Value << spriteRendererComponent.TilingFactor;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CircleRendererComponent>())
		{
			out << YAML::Key << "CircleRendererComponent";
			out << YAML::BeginMap;

			auto& circleRendererComponent = entity.GetComponent<CircleRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << circleRendererComponent.Color;
			out << YAML::Key << "Thickness" << YAML::Value << circleRendererComponent.Thickness;
			out << YAML::Key << "Fade" << YAML::Value << circleRendererComponent.Fade;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			out << YAML::Key << "Rigidbody2DComponent";
			out << YAML::BeginMap;

			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			out << YAML::Key << "Body Type" << YAML::Value << Rigidbody2DBodyTypeToString(rb2d.BType);
			out << YAML::Key << "Fixed Rotation" << YAML::Value << rb2d.fixedRotation;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap;

			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << bc2d.offset;
			out << YAML::Key << "Size" << YAML::Value << bc2d.size;
			out << YAML::Key << "Density" << YAML::Value << bc2d.Density;
			out << YAML::Key << "Friction" << YAML::Value << bc2d.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bc2d.Restitution;
			out << YAML::Key << "Restitution Threshold" << YAML::Value << bc2d.RestitutionThreshold;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			out << YAML::Key << "CircleCollider2DComponent";
			out << YAML::BeginMap;

			auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << cc2d.offset;
			out << YAML::Key << "Radius" << YAML::Value << cc2d.Radius;
			out << YAML::Key << "Density" << YAML::Value << cc2d.Density;
			out << YAML::Key << "Friction" << YAML::Value << cc2d.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << cc2d.Restitution;
			out << YAML::Key << "Restitution Threshold" << YAML::Value << cc2d.RestitutionThreshold;

			out << YAML::EndMap; // Circle Collider 2D Component
		}

		if (entity.HasComponent<TextComponent>())
		{
			out << YAML::Key << "TextComponent";
			out << YAML::BeginMap;

			auto& text = entity.GetComponent<TextComponent>();
			out << YAML::Key << "TextString" << YAML::Value << text.TextString;
			out << YAML::Key << "Color" << YAML::Value << text.Color;
			out << YAML::Key << "Kerning" << YAML::Value << text.Kerning;
			out << YAML::Key << "LineSpacing" << YAML::Value << text.LineSpacing;

			out << YAML::EndMap; // Text Component
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled Scene Name";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Registery.each([&](auto entityID)
			{
				Entity entity = { entityID, m_Scene.get() };
				if (!entity)
					return;

				SerializeEntity(out, entity);
			});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{

	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		YAML::Node data;

		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			MN_CORE_ERROR("Failed to load .modernengine file '{0}'\n", filepath, e.what());
			return false;
		}

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				MN_CORE_INFO("Name is: {0}", name);

				Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cp = deserializedEntity.AddComponent<CameraComponent>();
					auto& cameraProps = cameraComponent["Camera"];
					cp.m_Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cp.m_Camera.SetPerspectiveCameraFOV(cameraProps["PerspectiveFOV"].as<float>());
					cp.m_Camera.SetPerspectiveCameraNear(cameraProps["PerspectiveNear"].as<float>());
					cp.m_Camera.SetPerspectiveCameraFar(cameraProps["PerspectiveFar"].as<float>());

					cp.m_Camera.SetOrthographicCameraSize(cameraProps["OrthographicSize"].as<float>());
					cp.m_Camera.SetOrthographicCameraNear(cameraProps["OrthographicNear"].as<float>());
					cp.m_Camera.SetOrthographicCameraFar(cameraProps["OrthographicFar"].as<float>());

					cp.Primary = cameraComponent["Primary"].as<bool>();
					cp.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();

				}

				auto scriptComponent = entity["ScriptComponent"];
				if (scriptComponent)
				{
					auto& sc = deserializedEntity.AddComponent<ScriptComponent>();
					sc.ClassName = scriptComponent["ClassName"].as<std::string>();

					// Fields
					auto scriptFields = scriptComponent["ScriptFields"];
					if (scriptFields)
					{
						const auto& entityFields = ScriptEngine::GetEntityScriptClass(sc.ClassName)->GetFields();
						auto& scriptFieldInstanceMap = ScriptEngine::GetScriptFieldMap(deserializedEntity);
						for (auto scriptField : scriptFields)
						{
							std::string name = scriptField["Name"].as<std::string>();
							std::string typeName = scriptField["Type"].as<std::string>();
							ScriptClassFieldType type = Utils::ScriptClassFieldTypeFromString(typeName);
							ScriptFieldInstance& scriptFieldInstance = scriptFieldInstanceMap[name];

							if (scriptFieldInstanceMap.find(name) == scriptFieldInstanceMap.end())
								continue;

							scriptFieldInstance.Field = entityFields.at(name);
							switch (type)
							{
								READ_SCRIPT_FIELD(Float, float);
								READ_SCRIPT_FIELD(Double, double);
								READ_SCRIPT_FIELD(Bool, bool);
								READ_SCRIPT_FIELD(Char, char);
								READ_SCRIPT_FIELD(Byte, int8_t);
								READ_SCRIPT_FIELD(Short, int16_t);
								READ_SCRIPT_FIELD(Int, int32_t);
								READ_SCRIPT_FIELD(Long, int64_t);
								READ_SCRIPT_FIELD(UByte, uint8_t);
								READ_SCRIPT_FIELD(UShort, uint16_t);
								READ_SCRIPT_FIELD(UInt, uint32_t);
								READ_SCRIPT_FIELD(ULong, uint64_t);
								READ_SCRIPT_FIELD(Vector2, glm::vec2);
								READ_SCRIPT_FIELD(Vector3, glm::vec3);
								READ_SCRIPT_FIELD(Vector4, glm::vec4);
								READ_SCRIPT_FIELD(Entity, UUID);
							}
						}
					}
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& spriteRenderer = deserializedEntity.AddComponent<SpriteRendererComponent>();
					spriteRenderer.Color = spriteRendererComponent["Color"].as<glm::vec4>();

					if (spriteRendererComponent["TexturePath"])
					{
						std::string texturePath = spriteRendererComponent["TexturePath"].as<std::string>();
						spriteRenderer.Texture = Texture2D::Create(texturePath);
					}

					if (spriteRendererComponent["TilingFactor"])
						spriteRenderer.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
				}

				auto circleRendererComponent = entity["CircleRendererComponent"];
				if (circleRendererComponent)
				{
					auto& circleRenderer = deserializedEntity.AddComponent<CircleRendererComponent>();
					circleRenderer.Color = circleRendererComponent["Color"].as<glm::vec4>();
					circleRenderer.Thickness = circleRendererComponent["Thickness"].as<float>();
					circleRenderer.Fade = circleRendererComponent["Fade"].as<float>();
				}

				auto rb2dComponent = entity["Rigidbody2DComponent"];
				if (rb2dComponent)
				{
					auto& rb2d = deserializedEntity.AddComponent<Rigidbody2DComponent>();
					rb2d.BType = StringToRigidbody2DBodyType(rb2dComponent["Body Type"].as<std::string>());
					rb2d.fixedRotation = rb2dComponent["Fixed Rotation"].as<bool>();
				}

				auto bc2dComponent = entity["BoxCollider2DComponent"];
				if (bc2dComponent)
				{
					auto& bc2d = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					bc2d.offset = bc2dComponent["Offset"].as<glm::vec2>();
					bc2d.size = bc2dComponent["Size"].as<glm::vec2>();
					bc2d.Density = bc2dComponent["Density"].as<float>();
					bc2d.Friction = bc2dComponent["Friction"].as<float>();
					bc2d.Restitution = bc2dComponent["Restitution"].as<float>();
					bc2d.RestitutionThreshold = bc2dComponent["Restitution Threshold"].as<float>();
				}

				auto cc2dComponent = entity["CircleCollider2DComponent"];
				if (cc2dComponent)
				{
					auto& cc2d = deserializedEntity.AddComponent<CircleCollider2DComponent>();
					cc2d.offset = cc2dComponent["Offset"].as<glm::vec2>();
					cc2d.Radius = cc2dComponent["Radius"].as<float>();
					cc2d.Density = cc2dComponent["Density"].as<float>();
					cc2d.Friction = cc2dComponent["Friction"].as<float>();
					cc2d.Restitution = cc2dComponent["Restitution"].as<float>();
					cc2d.RestitutionThreshold = cc2dComponent["Restitution Threshold"].as<float>();
				}

				auto textComponent = entity["TextComponent"];
				if (textComponent)
				{
					auto& tc = deserializedEntity.AddComponent<TextComponent>();
					tc.TextString = textComponent["TextString"].as<std::string>();
					tc.Color = textComponent["Color"].as<glm::vec4>();
					tc.Kerning = textComponent["Kerning"].as<float>();
					tc.LineSpacing = textComponent["LineSpacing"].as<float>();
				}
			}
		}
		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		return false;
	}
}