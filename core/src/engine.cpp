#include "engine.h"
#include "logger/SpdLogManager.h"

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

			}

			Shutdown();
		}
	}

	bool Engine::Initialize()
	{
		mLogManager->Initialize();

		mLogManager->Trace("Engine initialized");

		return true;
	}

	void Engine::Shutdown()
	{
		mLogManager->Shutdown();
	}

	Engine::Engine()
	{
		mLogManager = new logger::SpdLogManager();
	}
}