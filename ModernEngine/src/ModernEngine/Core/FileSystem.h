#pragma once

#include "ModernEngine/Core/Buffer.h"

namespace ModernEngine {

	class FileSystem
	{
	public:
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};
}