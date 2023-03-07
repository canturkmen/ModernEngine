#include "mnpch.h"
#include "FileSystem.h"

namespace ModernEngine {

	Buffer FileSystem::ReadFileBinary(const std::filesystem::path& filepath)
	{
		std::ifstream inputFile(filepath, std::ios::binary | std::ios::ate);
		if (!inputFile)
			return Buffer();

		std::streampos end = inputFile.tellg();
		inputFile.seekg(0, std::ios::beg);
		uint64_t size = end - inputFile.tellg();

		if (size == 0)
			return Buffer();

		Buffer buffer(size);
		inputFile.read(buffer.As<char>(), size);
		inputFile.close();

		return buffer;
	}
}