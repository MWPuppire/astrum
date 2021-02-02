#ifndef INCLUDE_ASTRUM_WINDOW
#define INCLUDE_ASTRUM_WINDOW

extern "C" {
	#include "SDL.h"
}

#include <tuple>

#include "constants.hpp"

namespace Astrum {

namespace window {
	extern SDL_Window *window;

	int InitWindow(Config *game);
	void QuitWindow();
	void setFullscreen(bool fullscreen);
	void setSize(int width, int height);
	void setTitle(const char *title);
	bool getFullscreen();
	const char *getTitle();
	int getWidth();
	int getHeight();
	std::tuple<int, int> getDimensions();
	std::tuple<int, int> getDesktopDimensions();
	void setPosition(int x, int y);
	std::tuple<int, int> getPosition();
};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_WINDOW
