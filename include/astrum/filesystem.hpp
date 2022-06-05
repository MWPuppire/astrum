#ifndef INCLUDE_ASTRUM_FILESYSTEM
#define INCLUDE_ASTRUM_FILESYSTEM

#include <filesystem>

#include "constants.hpp"

namespace Astrum {

namespace filesystem
{
	int InitFS(Config &conf);
	void QuitFS();
	std::filesystem::path getAppDirectory();
	std::filesystem::path getSourceDirectory();
};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_FILESYSTEM
