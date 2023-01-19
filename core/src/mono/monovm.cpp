#include "mono/monovm.h"

namespace gem::mono
{
	void MonoVM::Initialize()
	{
		mono_set_dirs("F:\\Projects\\VS Projects\\GemEngine\\bin\\x64\\Debug\\", ".");

		mMonoDomain = mono_jit_init("MonoVM");


		if (mMonoDomain)
		{
			mMonoAssembly = mono_domain_assembly_open(mMonoDomain, "F:\\Projects\\VS Projects\\GemEngine\\bin\\x64\\Debug\\test_script.dll");

			if (mMonoAssembly)
			{
				mMonoImage = mono_assembly_get_image(mMonoAssembly);
				if (mMonoImage)
				{
					int i = 0;
				}
			}
		}
	}

	void MonoVM::Shutdown()
	{
		if (mMonoDomain)
		{
			mono_jit_cleanup(mMonoDomain);
		}
	}

	bool MonoVM::Tick()
	{
		return false;
	}
}
