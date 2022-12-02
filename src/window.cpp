#include <string>

#include "sdl.hpp"
#include "internals.hpp"
#include "astrum/constants.hpp"
#include "astrum/window.hpp"
#include "astrum/astrum.hpp"

namespace Astrum {

namespace window {
	SDL_Window *window;

	namespace {
		int windowHeight;
		int windowWidth;
	};

	int InitWindow(const Config &conf) {
		if (conf.existingWindow != nullptr) {
			window = (SDL_Window *) conf.existingWindow;
		} else {
			window = SDL_CreateWindow(
				conf.appName.c_str(),
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				conf.windowWidth,
				conf.windowHeight,
				SDL_WINDOW_OPENGL
					| (conf.windowFullscreen ? SDL_WINDOW_FULLSCREEN : 0)
					| (conf.windowResizable  ? SDL_WINDOW_RESIZABLE  : 0)
					| (conf.windowBorderless ? SDL_WINDOW_BORDERLESS : 0)
			);
		}

		if (window == nullptr) {
			log::error("Could not create window: %s\n", SDL_GetError());
			return -1;
		}
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);

		if (conf.windowResizable)
			SDL_SetWindowMinimumSize(window, conf.minWindowWidth, conf.minWindowHeight);

		if (!conf.icon.empty()) {
			Image image(conf.icon);
			std::shared_ptr<ImageData> data = image.getData();
			SDL_Surface *surf = data->image;
			SDL_SetWindowIcon(window, surf);
		}

		return 0;
	}

	void QuitWindow() {
		SDL_DestroyWindow(window);
	}

	void setFullscreen(bool fullscreen) {
		SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	}

	void setSize(int width, int height) {
		SDL_SetWindowSize(window, width, height);
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	}

	void setTitle(std::string title) {
		SDL_SetWindowTitle(window, title.c_str());
	}

	bool isFullscreen() {
		return (bool) (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN);
	}

	std::string getTitle() {
		return std::string(SDL_GetWindowTitle(window));
	}

	int getWidth() {
		return windowWidth;
	}

	int getHeight() {
		return windowHeight;
	}

	std::tuple<int, int> getDimensions() {
		return std::make_tuple(windowWidth, windowHeight);
	}

	std::tuple<int, int> getDesktopDimensions() {
		SDL_DisplayMode mode;
		int displayIdx = SDL_GetWindowDisplayIndex(window);
		SDL_GetDesktopDisplayMode(displayIdx, &mode);
		return std::make_tuple(mode.w, mode.h);
	}

	void setPosition(int x, int y) {

		SDL_SetWindowPosition(window, x, y);
	}

	std::tuple<int, int> getPosition() {
		int x, y;
		SDL_GetWindowPosition(window, &x, &y);
		return std::make_tuple(x, y);
	}

	int getMinWidth() {
		int w;
		SDL_GetWindowMinimumSize(window, &w, nullptr);
		return w;
	}

	int getMinHeight() {
		int h;
		SDL_GetWindowMinimumSize(window, nullptr, &h);
		return h;
	}

	std::tuple<int, int> getMinDimensions() {
		int w, h;
		SDL_GetWindowMinimumSize(window, &w, &h);
		return std::make_tuple(w, h);
	}

	bool isResizable() {
		return SDL_GetWindowFlags(window) & SDL_WINDOW_RESIZABLE
			? true : false;
	}

	void setResizable(bool toggle) {
		SDL_bool flag = toggle ? SDL_TRUE : SDL_FALSE;
		SDL_SetWindowResizable(window, flag);
	}

	void recalculateDimensions() {
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	}
};

}; // namespace Astrum
