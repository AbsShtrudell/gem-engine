#include "logger/SpdLogManager.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "logger/log.h"

namespace gem::logger
{
	void SpdLogManager::Initialize()
	{
		auto consoleSink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
		consoleSink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] %v%$");

		std::vector<spdlog::sink_ptr> sinks{ consoleSink };
		auto logger = std::make_shared<spdlog::logger>(GEM_DEFAULT_LOGGER_NAME, sinks.begin(), sinks.end());
		logger->set_level(spdlog::level::trace);
		logger->flush_on(spdlog::level::trace);
		spdlog::register_logger(logger);
	}

	void SpdLogManager::Shutdown()
	{
		spdlog::shutdown();
	}

	void SpdLogManager::Trace(std::string message)
	{
		if (spdlog::get(GEM_DEFAULT_LOGGER_NAME) != NULL)
		{
			spdlog::get(GEM_DEFAULT_LOGGER_NAME)->trace<std::string>(message);
		}
	}
}
