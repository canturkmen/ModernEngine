#pragma once

#include <stdint.h>
#include <cstring>

namespace ModernEngine {

	class Buffer
	{
	public:
		uint8_t* m_Data = nullptr;
		uint64_t m_Size = 0;

		Buffer(uint64_t size)
		{
			Allocate(size);
		}

		void Allocate(uint64_t size)
		{
			Release();

			m_Data = new uint8_t[size];
			m_Size = size;
		}

		void Release()
		{
			delete m_Data;
			m_Data = nullptr;
			m_Size = 0;
		}
	};

}