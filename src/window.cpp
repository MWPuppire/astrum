extern "C" {
	#include "SDL.h"
	#include "SDL_gpu.h"
}

#include <tuple>

#include "astrum/constants.hpp"
#include "astrum/window.hpp"
#include "astrum/astrum.hpp"

namespace Astrum
{

namespace window
{
	SDL_Window *window;

	namespace
	{
		Config *settings;

		int windowHeight;
		int windowWidth;
	};

	int InitWindow(Config *conf)
	{
		settings = conf;
		if (conf->windowHeadless) {
			window = NULL;
			return -1;
		}

		window = SDL_CreateWindow(
			conf->windowTitle,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			conf->windowWidth,
			conf->windowHeight,
			SDL_WINDOW_OPENGL
				| (conf->windowFullscreen ? SDL_WINDOW_FULLSCREEN : 0)
				| (conf->windowResizable  ? SDL_WINDOW_RESIZABLE  : 0)
				| (conf->windowBorderless ? SDL_WINDOW_BORDERLESS : 0)
		);

		if (window == NULL) {
			SDL_Log("Could not create window: %s\n", SDL_GetError());
			if (conf->allowNoWindow)
				return 0;
			else
				return -1;
		}
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);

		if (conf->windowResizable)
			SDL_SetWindowMinimumSize(window, conf->minWindowWidth, conf->minWindowHeight);

		auto resizeevent = [](int w, int h) { windowWidth = w; windowHeight = h; };
		onresize(resizeevent);

		return 0;
	}

	void QuitWindow()
	{
		if (window == NULL)
			return;

		SDL_DestroyWindow(window);
	}

	void setFullscreen(bool fullscreen)
	{
		if (window == NULL)
			return;

		SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	}

	void setSize(int width, int height)
	{
		if (window == NULL)
			return;

		SDL_SetWindowSize(window, width, height);
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	}

	void setTitle(const char *title)
	{
		if (window == NULL)
			return;

		SDL_SetWindowTitle(window, title);
	}

	bool getFullscreen()
	{
		if (window == NULL)
			return false;

		return (bool) (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN);
	}

	const char *getTitle()
	{
		if (window == NULL)
			return "";

		return SDL_GetWindowTitle(window);
	}

	int getWidth()
	{
		if (window == NULL)
			return 0;

		return windowWidth;
	}

	int getHeight()
	{
		if (window == NULL)
			return 0;

		return windowHeight;
	}

	std::tuple<int, int> getDimensions()
	{
		if (window == NULL)
			return std::make_tuple(0, 0);

		return std::make_tuple(windowWidth, windowHeight);
	}

	std::tuple<int, int> getDesktopDimensions()
	{
		if (window == NULL) {
			SDL_DisplayMode mode;
			SDL_GetDesktopDisplayMode(0, &mode);
			return std::make_tuple(mode.w, mode.h);
		}

		SDL_DisplayMode mode;
		int displayIdx = SDL_GetWindowDisplayIndex(window);
		SDL_GetDesktopDisplayMode(displayIdx, &mode);
		return std::make_tuple(mode.w, mode.h);
	}

	void setPosition(int x, int y)
	{
		if (window == NULL)
			return;

		SDL_SetWindowPosition(window, x, y);
	}

	std::tuple<int, int> getPosition()
	{
		if (window == NULL)
			return std::make_tuple(0, 0);

		int x;
		int y;
		SDL_GetWindowPosition(window, &x, &y);
		return std::make_tuple(x, y);
	}

};

}; // namespace Astrum