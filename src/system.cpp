#include <string>

#include "sdl.hpp"
#include "internals.hpp"
#include "astrum/constants.hpp"
#include "astrum/system.hpp"

namespace Astrum {

namespace system {

	std::string getPlatformName() {
		return std::string(SDL_GetPlatform());
	}

	bool openURL(std::string url) {
		return SDL_OpenURL(url.c_str()) == 0;
	}

	bool hasClipboardText() {
		return SDL_HasClipboardText() == SDL_TRUE;
	}
	std::string getClipboardText() {
		return SDL_GetClipboardText();
	}
	void setClipboardText(std::string text) {
		SDL_SetClipboardText(text.c_str());
	}

}

}; // namespace Astrum
