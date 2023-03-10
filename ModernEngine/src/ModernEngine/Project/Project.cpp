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
			s_ActiveProject = project;
			return s_ActiveProject;
		}

		return nullptr;
	}

	bool Project::Save(const std::filesystem::path& filepath)
	{
		ProjectSerializer deserializer(s_ActiveProject);
		if (deserializer.Serialize(filepath))
			return true;

		return false;
	}

}