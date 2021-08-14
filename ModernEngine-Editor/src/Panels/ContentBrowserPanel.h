#pragma once

#include <filesystem>
#include "ModernEngine/Renderer/Texture.h"

namespace ModernEngine {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		void OnImGuiRender();
	private:
		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_FileIcon;

		std::filesystem::path m_CurrentDirectory;
	};
}

