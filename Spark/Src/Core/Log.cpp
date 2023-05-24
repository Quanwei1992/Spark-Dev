#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <filesystem>

std::shared_ptr<spdlog::logger> Spark::Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Spark::Log::s_ClientLogger;

void Spark::Log::Init()
{
	// Create "logs" directory if doesn't exist
	const std::string logsDirectory = "logs";

	if (!std::filesystem::exists(logsDirectory))
		std::filesystem::create_directories(logsDirectory);

	std::vector<spdlog::sink_ptr> engineSinks =
	{
		std::make_shared<spdlog::sinks::basic_file_sink_mt>("Logs/Spark.log", true),
		std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
	};

	std::vector<spdlog::sink_ptr> appSinks =
	{
		std::make_shared<spdlog::sinks::basic_file_sink_mt>("Logs/App.log", true),
		std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
	};

	engineSinks[0]->set_pattern("[%T] [%l] %n: %v");
	appSinks[0]->set_pattern("[%T] [%l] %n: %v");

	engineSinks[1]->set_pattern("%^[%T] %n: %v%$");
	appSinks[1]->set_pattern("%^[%T] %n: %v%$");


	s_CoreLogger = std::make_shared<spdlog::logger>("Spark", engineSinks.begin(), engineSinks.end());
	s_CoreLogger->set_level(spdlog::level::trace);

	s_ClientLogger = std::make_shared<spdlog::logger>("APP", appSinks.begin(), appSinks.end());
	s_ClientLogger->set_level(spdlog::level::trace);
}

void Spark::Log::Shutdown()
{
	s_ClientLogger.reset();
	s_CoreLogger.reset();
	spdlog::drop_all();
}
