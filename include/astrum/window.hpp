#ifndef INCLUDE_ASTRUM_WINDOW
#define INCLUDE_ASTRUM_WINDOW

extern "C" {
	#define SDL_MAIN_HANDLED
	#include <SDL2/SDL.h>
}

#include <tuple>

#include "constants.hpp"

namespace Astrum {

namespace window {
	extern SDL_Window *window;

	int InitWindow(Config *conf);
	void QuitWindow();
	void setFullscreen(bool fullscreen);
	void setSize(int width, int height);
	void setTitle(const char *title);
	bool getFullscreen();
	const char *getTitle();
	int getWidth();
	int getHeight();
	std::tuple<int, int> getDimensions();
	void getDimensions(int *w, int *h);
	std::tuple<int, int> getDesktopDimensions();
	void getDesktopDimensions(int *w, int *h);
	void setPosition(int x, int y);
	std::tuple<int, int> getPosition();
	void getPosition(int *x, int *y);
	int getMinWidth();
	int getMinHeight();
	std::tuple<int, int> getMinDimensions();
	void getMinDimensions(int *w, int *h);
	bool isResizable();
	void setResizable(bool toggle);
};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_WINDOW
