#pragma once
#include "LogManager.h"

namespace gem::logger
{
	class SpdLogManager : public LogManager
	{
	public:
		SpdLogManager() = default;
		virtual ~SpdLogManager() = default;

		virtual void Initialize() override;
		virtual void Shutdown() override;

		virtual void Trace(std::string message) override;
	};
}