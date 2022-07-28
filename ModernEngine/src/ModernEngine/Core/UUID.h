#pragma once

namespace ModernEngine {

	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(UUID& other) = default;

		operator uint64_t() const { return m_UUID; }

	private:
		uint64_t m_UUID; 
	};
}

namespace std {

	template <typename T> struct hash;

	template<>
	struct hash<ModernEngine::UUID>
	{
		size_t operator() (const ModernEngine::UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};
	
}