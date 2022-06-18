#ifndef INCLUDE_ASTRUM_UTIL
#define INCLUDE_ASTRUM_UTIL

#include <cstdarg>

#include "constants.hpp"

namespace Astrum {

/**
 * @brief General-purpose utility functions that don't fit anywhere else.
 *
 * Namespace for miscellaneous utility functions. Includes functions for
 * formatting strings.
 */
namespace util {

	/**
	 * @brief Create a formatted string.
	 *
	 * Creates and returns a formatted string, using printf format strings.
	 * Takes the format string and any number of parameters to use in
	 * formatting.
	 *
	 * @param format The format string to use.
	 * @return A printf-formatted string using the arguments.
	 */
	std::string strformat(std::string format, ...);

	/**
	 * @brief Create a formatted string using a `va_list`.
	 *
	 * Creates and returns a formatted string, using printf format strings.
	 * Takes the format string and a `va_list` containing all parameters for
	 * formatting.
	 *
	 * @param format The format string to use.
	 * @param args A `va_list` containing all arguments to format with.
	 * @return A printf-formatted string using the arguments.
	 */
	std::string vstrformat(std::string format, std::va_list args);

}

}; // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_UTIL
