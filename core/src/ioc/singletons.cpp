#include "ioc/singletons.h"

namespace gem::ioc
{
	Singletons& Singletons::Get()
	{
		static Singletons s;
		return s;
	}
}