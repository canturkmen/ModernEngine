#pragma once

#include <string>
#include <optional>

namespace ModernEngine {

	class FileDialogs
	{
	public:
		static std::optional<std::string> OpenFile(const char* filter);
		static std::optional<std::string> SaveFile(const char* filter);
	};
		
}