#pragma once

#include <string>

namespace gem::logger
{
	class LogManager
	{
	public:
		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;

		virtual void Trace(std::string message) = 0;
	};
}

