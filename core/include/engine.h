#pragma once

#include "logger/LogManager.h"
#include "mono/monovm.h"

namespace gem
{
	class Engine
	{
	public:
		static Engine& Instance();
		~Engine() {};

		void Run();
		inline void Quit() { mIsRunning = false; };

	private:
		[[nodiscard]] bool Initialize();
		void Shutdown();

	private:
		bool mIsRunning;

		logger::LogManager* mLogManager;
		mono::MonoVM* mMonoVM;

		Engine();
		static Engine* mInstance;
	};
}
