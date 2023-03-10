#include "mnpch.h"
#include "ProjectSerializer.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace ModernEngine {

	ProjectSerializer::ProjectSerializer(Ref<Project> project)
		: m_Project(project)
	{
		
	}

	bool ProjectSerializer::Serialize(const std::filesystem::path& path)
	{
		const auto& config = m_Project->GetConfig();

		YAML::Emitter out;
		
		out << YAML::BeginMap;
		out << YAML::Key << "Project" << YAML::Value;
		
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << config.Name;
		out << YAML::Key << "StartScene" << YAML::Value << config.StartScene.string();
		out << YAML::Key << "AssetDirectory" << YAML::Value << config.AssetDirectory.string();
		out << YAML::Key << "ScriptModulePath" << YAML::Value << config.ScriptModulePath.string();
		out << YAML::EndMap;
		
		out << YAML::EndMap;		

		std::ofstream fout(path);
		fout << out.c_str();

		return true;
	}

	bool ProjectSerializer::Deserialize(const std::filesystem::path& path)
	{
		auto& config = m_Project->GetConfig();

		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path.string());
		}
		catch (YAML::ParserException e)
		{
			MN_CORE_ERROR("Failed to load project file '{0}'\n", path, e.what());
			return false;
		}

		auto projectNode = data["Project"];

		if (!projectNode)
			return false;

		config.Name = projectNode["Name"].as<std::string>();
		config.StartScene = projectNode["StartScene"].as<std::string>();
		config.AssetDirectory = projectNode["AssetDirectory"].as<std::string>();
		config.ScriptModulePath = projectNode["ScriptModulePath"].as<std::string>();

		return true;
	}
}