#pragma once

#include <string>
#include <filesystem>

#include "ModernEngine/Core/Base.h"

namespace ModernEngine {

	struct ProjectConfig
	{
		std::string Name = "Untitled";

		std::filesystem::path StartScene;

		std::filesystem::path AssetDirectory;
		std::filesystem::path ScriptModulePath;
	};

	class Project
	{
	public:
		Project() = default;	

		Project(const ProjectConfig& settings)
			:m_Settings(settings) {}

		static const std::filesystem::path& GetProjectDirectory() { return s_ActiveProject->m_ProjectDirectory; }
		static std::filesystem::path GetAssetDirectory() { return GetProjectDirectory() / s_ActiveProject->m_Settings.AssetDirectory; }
		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path) { return GetAssetDirectory() / path; }

		ProjectConfig& GetConfig() { return m_Settings; }
;		static Ref<Project> GetActive() { return s_ActiveProject; }
		static Ref <Project> New();
		static Ref<Project> Load(const std::filesystem::path& filepath);
		bool Save(const std::filesystem::path& filepath);

	private:
		ProjectConfig m_Settings;
		std::filesystem::path m_ProjectDirectory;

		inline static Ref<Project> s_ActiveProject;
	};
}