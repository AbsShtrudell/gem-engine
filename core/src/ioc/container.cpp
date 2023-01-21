#include "ioc/container.h"

namespace gem::ioc
{
	Container& Container::Get()
	{
		static Container c;
		return c;
	}
}