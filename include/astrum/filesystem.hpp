#ifndef INCLUDE_ASTRUM_FILESYSTEM
#define INCLUDE_ASTRUM_FILESYSTEM

#include <filesystem>

#include "constants.hpp"

namespace Astrum {

/**
 * @brief Contains cross-platform filesystem utilities
 *
 * Namespace for filesystem utilities. Currently holds cross-platform directory
 * paths for useful directories.
 */
namespace filesystem {

	/**
	 * @brief Get the OS-specific application save directory
	 *
	 * Get a path to a directory, platform-dependent, where the application can
	 * save and load data as needed by the app. This is the Application Support
	 * directory on macOS, %APPDATA% on Windows, etc.
	 *
	 * @return std::filesystem::path app_dir
	 */
	std::filesystem::path getAppDirectory();
	/**
	 * @brief Get the base directory path
	 *
	 * Get the path to where the application was opened from.
	 *
	 * @return std::filesystem::path base_dir
	 */
	std::filesystem::path getSourceDirectory();
};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_FILESYSTEM
