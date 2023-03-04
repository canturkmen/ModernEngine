#pragma once

#include <filesystem>
#include "ModernEngine/Core/Base.h"
#include "ModernEngine/Renderer/Texture.h"

namespace ModernEngine {

	struct MSDFData;

	class Font
	{
	public:
		Font(const std::filesystem::path& filePath);
		~Font();

		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }
		const MSDFData* GetMSDFData() const { return m_Data; }

		static Ref<Font> GetDefault();

	private:
		MSDFData* m_Data;
		Ref<Texture2D> m_AtlasTexture;
	};
}