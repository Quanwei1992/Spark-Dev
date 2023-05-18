#pragma once

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>

namespace Spark
{
	class Log
	{
	public:
		static void Init();
		static void Shutdown();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}


// Core log macros
#define SP_CORE_TRACE(...)		::Spark::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SP_CORE_INFO(...)		::Spark::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SP_CORE_WARN(...)		::Spark::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SP_CORE_ERROR(...)		::Spark::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SP_CORE_FATAL(...)		::Spark::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Core client macros
#define SP_TRACE(...)			::Spark::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SP_INFO(...)			::Spark::Log::GetClientLogger()->info(__VA_ARGS__)
#define SP_WARN(...)			::Spark::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SP_ERROR(...)			::Spark::Log::GetClientLogger()->error(__VA_ARGS__)
#define SP_FATAL(...)			::Spark::Log::GetClientLogger()->critical(__VA_ARGS__)

#pragma warning(pop)
