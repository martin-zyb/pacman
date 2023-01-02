#include "uni.h"

namespace uni
{

	// Universal sleep() function in millisecond
	void __sleep(unsigned int seconds)
	{
		#ifdef _WIN32
			Sleep(seconds);
		#elif __linux__
			seconds \ = 1000;
			sleep(seconds);
		#endif
	}

}