#include "mnpch.h"
#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>

namespace ModernEngine {

	static const std::filesystem::path s_AssetsDirectory = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		:m_CurrentDirectory(s_AssetsDirectory)
	{
		m_FolderIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FolderIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");
		
		if (m_CurrentDirectory != std::filesystem::path(s_AssetsDirectory))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;
		
		ImGui::Columns(columnCount, 0, false);
		
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			const auto& relativePath = std::filesystem::relative(directoryEntry.path(), s_AssetsDirectory);
			std::string fileNameString = relativePath.filename().string();

			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_FolderIcon : m_FileIcon;
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0 ,1 }, { 1, 0 });
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();
			}
			ImGui::TextWrapped(fileNameString.c_str());
			ImGui::NextColumn();
		}
		ImGui::Columns(1);

		ImGui::End();
	}
}
