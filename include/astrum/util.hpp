#ifndef INCLUDE_ASTRUM_UTIL
#define INCLUDE_ASTRUM_UTIL

#include <cstdarg>

#include "constants.hpp"

namespace Astrum {

namespace util {

	std::string strformat(std::string format, ...);
	std::string vstrformat(std::string format, std::va_list args);

}

}; // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_UTIL
