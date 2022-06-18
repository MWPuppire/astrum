extern "C" {
	#define SDL_MAIN_HANDLED
	#include <SDL2/SDL.h>
}

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#endif

#include <filesystem>
#include <string>

#include "astrum/filesystem.hpp"
#include "astrum/log.hpp"

#ifdef __EMSCRIPTEN__
extern "C"
{
	void log_fs_error()
	{
		Astrum::log::error("Could not establish persistent memory. "
			"No data will be saved across program runs\n");
	}
}
#endif

namespace Astrum {

namespace filesystem
{
	namespace
	{
		std::filesystem::path appDirectory;
		std::filesystem::path sourceDirectory;
	};
	int InitFS(Config &conf)
	{
#ifdef __EMSCRIPTEN__
		(void) conf;
		appDirectory = std::filesystem::path("/offline");
		sourceDirectory = std::filesystem::path("/");
		EM_ASM(
			FS.mkdir("/offline");
			FS.mount(IDBFS, { }, "/offline");
			FS.syncfs(true, function(err) {
				if (err) {
					ccall("log_fs_error", "v");
				}
			});
		);
		return 0;
#else
		char *rawstr;
		std::string str;
		rawstr = SDL_GetPrefPath(conf.orgName.c_str(),
			conf.appName.c_str());
		if (rawstr == nullptr) {
			log::error("Could not get app directory: %s\n",
				SDL_GetError());
		} else {
			str = std::string(rawstr);
			appDirectory = std::filesystem::path(str);
			SDL_free((void *) rawstr);
		}
		rawstr = SDL_GetBasePath();
		if (rawstr == nullptr) {
			log::error("Could not get source directory: %s\n",
				SDL_GetError());
		} else {
			str = std::string(rawstr);
			sourceDirectory = std::filesystem::path(str);
			SDL_free((void *) rawstr);
		}
		return 0;
#endif
	}
	void QuitFS()
	{
#ifdef __EMSCRIPTEN__
		EM_ASM(
			FS.syncfs(false, function(err) {
				if (err) {
					ccall("log_fs_error", "v");
				}
			});
			FS.unmount("/offline");
		);
#endif
	}
	std::filesystem::path getAppDirectory()
	{
		return appDirectory;
	}
	std::filesystem::path getSourceDirectory()
	{
		return sourceDirectory;
	}
};

} // namespace Astrum
