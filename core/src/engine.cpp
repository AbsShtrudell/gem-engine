#include "engine.h"
#include "logger/SpdLogManager.h"

#include <mono/metadata/assembly.h>

namespace gem
{
	Engine* Engine::mInstance = nullptr;

	Engine& Engine::Instance()
	{
		if (!mInstance)
			mInstance = new Engine();

		return *mInstance;
	}

	void Engine::Run()
	{
		if (Initialize())
		{
			while (mIsRunning)
			{
				mMonoVM->Tick();
			}

			Shutdown();
		}
	}

	bool Engine::Initialize()
	{
		mLogManager->Initialize();
		mMonoVM->Initialize();

		mLogManager->Trace("Engine initialized");

		return true;
	}

	void Engine::Shutdown()
	{
		mLogManager->Shutdown();
		mMonoVM->Shutdown();
	}

	Engine::Engine()
	{
		mIsRunning = true;

		mLogManager = new logger::SpdLogManager();
		mMonoVM = new mono::MonoVM();
	}
}