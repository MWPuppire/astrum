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
		if (conf.windowHeadless) {
			window = nullptr;
			return 0;
		}

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
			log::warn("Could not create window: %s\n", SDL_GetError());
			if (conf.allowNoWindow)
				return 0;
			else
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
		if (window == nullptr)
			return;

		SDL_DestroyWindow(window);
	}

	void setFullscreen(bool fullscreen) {
		if (window == nullptr)
			return;

		SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	}

	void setSize(int width, int height) {
		if (window == nullptr)
			return;

		SDL_SetWindowSize(window, width, height);
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	}

	void setTitle(std::string title) {
		if (window == nullptr)
			return;

		SDL_SetWindowTitle(window, title.c_str());
	}

	bool isFullscreen() {
		if (window == nullptr)
			return false;

		return (bool) (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN);
	}

	std::string getTitle() {
		if (window == nullptr)
			return "";

		return std::string(SDL_GetWindowTitle(window));
	}

	int getWidth() {
		if (window == nullptr)
			return 0;

		return windowWidth;
	}

	int getHeight() {
		if (window == nullptr)
			return 0;

		return windowHeight;
	}

	std::tuple<int, int> getDimensions() {
		if (window == nullptr)
			return std::make_tuple(0, 0);

		return std::make_tuple(windowWidth, windowHeight);
	}

	std::tuple<int, int> getDesktopDimensions() {
		if (window == nullptr) {
			SDL_DisplayMode mode;
			SDL_GetDesktopDisplayMode(0, &mode);
			return std::make_tuple(mode.w, mode.h);
		}

		SDL_DisplayMode mode;
		int displayIdx = SDL_GetWindowDisplayIndex(window);
		SDL_GetDesktopDisplayMode(displayIdx, &mode);
		return std::make_tuple(mode.w, mode.h);
	}

	void setPosition(int x, int y) {
		if (window == nullptr)
			return;

		SDL_SetWindowPosition(window, x, y);
	}

	std::tuple<int, int> getPosition() {
		if (window == nullptr)
			return std::make_tuple(0, 0);

		int x, y;
		SDL_GetWindowPosition(window, &x, &y);
		return std::make_tuple(x, y);
	}

	int getMinWidth() {
		if (window == nullptr)
			return 0;

		int w;
		SDL_GetWindowMinimumSize(window, &w, nullptr);
		return w;
	}

	int getMinHeight() {
		if (window == nullptr)
			return 0;

		int h;
		SDL_GetWindowMinimumSize(window, nullptr, &h);
		return h;
	}

	std::tuple<int, int> getMinDimensions() {
		if (window == nullptr)
			return std::make_tuple(0, 0);

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
