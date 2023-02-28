#pragma once

#include <filesystem>
#include "ModernEngine/Core/Base.h"

namespace ModernEngine {

	struct MSDFData;

	class Font
	{
	public:
		Font(const std::filesystem::path& filePath);
		~Font();

	private:
		MSDFData* m_Data;
	};
}