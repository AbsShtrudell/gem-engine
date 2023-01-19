#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace gem::mono
{
	class MonoVM
	{
	public:
		MonoVM() = default;
		~MonoVM() = default;

		void Initialize();
		void Shutdown();

		bool Tick();

	private:
		MonoDomain* mMonoDomain;
		MonoAssembly* mMonoAssembly;
		MonoImage* mMonoImage;
	};
}