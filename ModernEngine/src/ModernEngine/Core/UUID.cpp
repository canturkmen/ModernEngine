#include "mnpch.h"
#include "UUID.h"

#include <random>
#include <unordered_map>

namespace ModernEngine {
	
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution s_UniformDistribution;

	UUID::UUID()
		: m_UUID(s_UniformDistribution(s_Engine))
	{
		
	}

	UUID::UUID(uint64_t uuid)
	{

	}

}