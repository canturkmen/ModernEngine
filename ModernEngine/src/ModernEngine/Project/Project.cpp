#include "mnpch.h"
#include "Project.h"

#include "ProjectSerializer.h"

namespace ModernEngine {

	Ref<Project> Project::New()
	{
		s_ActiveProject = CreateRef<Project>();
		return s_ActiveProject;
	}

	Ref<Project> Project::Load(const std::filesystem::path& filepath)
	{
		Ref<Project> project = CreateRef<Project>();
		ProjectSerializer serializer(project);
		if (serializer.Deserialize(filepath))
		{
			project->m_ProjectDirectory = filepath.parent_path();
			s_ActiveProject = project;
			return s_ActiveProject;
		}

		return nullptr;
	}

	bool Project::Save(const std::filesystem::path& filepath)
	{
		ProjectSerializer serializer(s_ActiveProject);
		if (serializer.Serialize(filepath))
		{
			s_ActiveProject->m_ProjectDirectory = filepath.parent_path();
			return true;
		}

		return false;
	}

}