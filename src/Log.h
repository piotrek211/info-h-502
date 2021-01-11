#pragma once

#include <spdlog/spdlog.h>
#include "Basic.h"

namespace GEngine {
	class Log {
	public:
		static void init();
		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};

	// Core log macros
#define GE_CORE_TRACE(...)    ::GEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GE_CORE_INFO(...)     ::GEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GE_CORE_WARN(...)     ::GEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GE_CORE_ERROR(...)    ::GEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GE_CORE_CRITICAL(...) ::GEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define GE_CORE_ASSERT(test, text) {if(test == 0) {GE_CORE_ERROR(text);}}

// Client log macros
#define GE_TRACE(...)         ::GEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GE_INFO(...)          ::GEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define GE_WARN(...)          ::GEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GE_ERROR(...)         ::GEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define GE_CRITICAL(...)      ::GEngine::Log::GetClientLogger()->critical(__VA_ARGS__)
}