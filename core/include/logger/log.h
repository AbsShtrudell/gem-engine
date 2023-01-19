#pragma once

#define GEM_DEFAULT_LOGGER_NAME "GemLogger"

#ifndef NDEBUG
	#define GEM_TRACE(...) 
#else
	#define GEM_TRACE(...) (void)0
#endif