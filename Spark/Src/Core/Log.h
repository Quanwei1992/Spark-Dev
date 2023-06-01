#pragma once

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <Windows.h>
namespace Spark
{
	class Log
	{

	public:
		enum class Type : uint8_t
		{
			Core = 0,Client
		};
		enum class Level : uint8_t
		{
			Trace=0,Info,Warn,Error,Fatal
		};
		struct TagDetails
		{
			bool Enabled = true;
			Level LevelFilter = Level::Trace;
		};

	public:
		static void Init();
		static void Shutdown();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

		static bool HasTag(const std::string& tag) { return s_EnabledTags.find(tag) != s_EnabledTags.end(); }
		static std::unordered_map<std::string, TagDetails>& EnableTags() { return s_EnabledTags; }

		template<typename... Args>
		static void PrintMessage(Log::Type type, Log::Level level, std::string_view tag, Args&&... args);

		template<typename... Args>
		static void PrintAssertMessage(Log::Type type, std::string_view prefix, Args&&... args);

	public:
		static const char* LevelToString(Level level)
		{
			switch (level)
			{
			case Level::Trace: return "Trace";
			case Level::Info:  return "Info";
			case Level::Warn:  return "Warn";
			case Level::Error: return "Error";
			case Level::Fatal: return "Fatal";
			}
			return "";
		}

		static Level LevelFromString(std::string_view string)
		{
			if (string == "Trace") return Level::Trace;
			if (string == "Info")  return Level::Info;
			if (string == "Warn")  return Level::Warn;
			if (string == "Error") return Level::Error;
			if (string == "Fatal") return Level::Fatal;

			return Level::Trace;
		}

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		inline static std::unordered_map<std::string, TagDetails> s_EnabledTags;
	};

}


// Core logging
#define SP_CORE_TRACE_TAG(tag, ...) ::Spark::Log::PrintMessage(::Spark::Log::Type::Core, ::Spark::Log::Level::Trace, tag, __VA_ARGS__)
#define SP_CORE_INFO_TAG(tag, ...)  ::Spark::Log::PrintMessage(::Spark::Log::Type::Core, ::Spark::Log::Level::Info, tag, __VA_ARGS__)
#define SP_CORE_WARN_TAG(tag, ...)  ::Spark::Log::PrintMessage(::Spark::Log::Type::Core, ::Spark::Log::Level::Warn, tag, __VA_ARGS__)
#define SP_CORE_ERROR_TAG(tag, ...) ::Spark::Log::PrintMessage(::Spark::Log::Type::Core, ::Spark::Log::Level::Error, tag, __VA_ARGS__)
#define SP_CORE_FATAL_TAG(tag, ...) ::Spark::Log::PrintMessage(::Spark::Log::Type::Core, ::Spark::Log::Level::Fatal, tag, __VA_ARGS__)

// Client logging
#define SP_TRACE_TAG(tag, ...) ::Spark::Log::PrintMessage(::Spark::Log::Type::Client, ::Spark::Log::Level::Trace, tag, __VA_ARGS__)
#define SP_INFO_TAG(tag, ...)  ::Spark::Log::PrintMessage(::Spark::Log::Type::Client, ::Spark::Log::Level::Info, tag, __VA_ARGS__)
#define SP_WARN_TAG(tag, ...)  ::Spark::Log::PrintMessage(::Spark::Log::Type::Client, ::Spark::Log::Level::Warn, tag, __VA_ARGS__)
#define SP_ERROR_TAG(tag, ...) ::Spark::Log::PrintMessage(::Spark::Log::Type::Client, ::Spark::Log::Level::Error, tag, __VA_ARGS__)
#define SP_FATAL_TAG(tag, ...) ::Spark::Log::PrintMessage(::Spark::Log::Type::Client, ::Spark::Log::Level::Fatal, tag, __VA_ARGS__)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Core Logging
#define SP_CORE_TRACE(...)  ::Spark::Log::PrintMessage(::Spark::Log::Type::Core, ::Spark::Log::Level::Trace, "", __VA_ARGS__)
#define SP_CORE_INFO(...)   ::Spark::Log::PrintMessage(::Spark::Log::Type::Core, ::Spark::Log::Level::Info, "", __VA_ARGS__)
#define SP_CORE_WARN(...)   ::Spark::Log::PrintMessage(::Spark::Log::Type::Core, ::Spark::Log::Level::Warn, "", __VA_ARGS__)
#define SP_CORE_ERROR(...)  ::Spark::Log::PrintMessage(::Spark::Log::Type::Core, ::Spark::Log::Level::Error, "", __VA_ARGS__)
#define SP_CORE_FATAL(...)  ::Spark::Log::PrintMessage(::Spark::Log::Type::Core, ::Spark::Log::Level::Fatal, "", __VA_ARGS__)

// Client Logging
#define SP_TRACE(...)   ::Spark::Log::PrintMessage(::Spark::Log::Type::Client, ::Spark::Log::Level::Trace, "", __VA_ARGS__)
#define SP_INFO(...)    ::Spark::Log::PrintMessage(::Spark::Log::Type::Client, ::Spark::Log::Level::Info, "", __VA_ARGS__)
#define SP_WARN(...)    ::Spark::Log::PrintMessage(::Spark::Log::Type::Client, ::Spark::Log::Level::Warn, "", __VA_ARGS__)
#define SP_ERROR(...)   ::Spark::Log::PrintMessage(::Spark::Log::Type::Client, ::Spark::Log::Level::Error, "", __VA_ARGS__)
#define SP_FATAL(...)   ::Spark::Log::PrintMessage(::Spark::Log::Type::Client, ::Spark::Log::Level::Fatal, "", __VA_ARGS__)
#pragma warning(pop)


namespace Spark
{
	template<typename... Args>
	void Log::PrintMessage(Log::Type type, Log::Level level, std::string_view tag, Args&&... args)
	{
		auto detail = s_EnabledTags[std::string(tag)];
		if (detail.Enabled && detail.LevelFilter <= level)
		{
			auto logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
			std::string logString = tag.empty() ? "{0}{1}" : "[{0}] {1}";
			switch (level)
			{
			case Level::Trace:
				logger->trace(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Info:
				logger->info(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Warn:
				logger->warn(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Error:
				logger->error(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Fatal:
				logger->critical(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			}
		}
	}


	template<typename... Args>
	void Log::PrintAssertMessage(Log::Type type, std::string_view prefix, Args&&... args)
	{
		auto logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
		logger->error("{0}: {1}", prefix, fmt::format(std::forward<Args>(args)...));


	}

	template<>
	inline void Log::PrintAssertMessage(Log::Type type, std::string_view prefix)
	{
		auto logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
		logger->error("{0}", prefix);
	}
}
