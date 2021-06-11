#pragma once

#ifdef MN_PLATFORM_WINDOWS
	#ifdef MN_BUILD_DLL 
		#define MN_API __declspec(dllexport)
	#else
		#define MN_API __declspec(dllimport)
	#endif
#else 
	#error Modern Engine only supports Windows !
#endif

#define BIT(x) (1 << x)