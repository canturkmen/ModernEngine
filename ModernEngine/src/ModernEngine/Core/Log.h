#pragma once

#include "Base.h"

// Ignore the warnings
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)

namespace ModernEngine {

	class Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core logging macros
#define MN_CORE_TRACE(...)		ModernEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MN_CORE_WARN(...)		ModernEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MN_CORE_ERROR(...)		ModernEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MN_CORE_INFO(...)		ModernEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MN_CORE_CRITICAL(...)	ModernEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client logging macros
#define MN_TRACE(...)			ModernEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MN_WARN(...)			ModernEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MN_ERROR(...)			ModernEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define MN_INFO(...)			ModernEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define MN_CRITICAL(...)		ModernEngine::Log::GetClientLogger()->critical(__VA_ARGS__)



