#ifndef INCLUDE_ASTRUM_WINDOW
#define INCLUDE_ASTRUM_WINDOW

#include <tuple>
#include <string>

#include "constants.hpp"

namespace Astrum {

namespace window {
	int InitWindow(Config &conf);
	void QuitWindow();
	void setFullscreen(bool fullscreen);
	void setSize(int width, int height);
	void setTitle(std::string title);
	bool getFullscreen();
	std::string getTitle();
	int getWidth();
	int getHeight();
	std::tuple<int, int> getDimensions();
	void getDimensions(int &width, int &height);
	std::tuple<int, int> getDesktopDimensions();
	void getDesktopDimensions(int &width, int &height);
	void setPosition(int x, int y);
	std::tuple<int, int> getPosition();
	void getPosition(int &x, int &y);
	int getMinWidth();
	int getMinHeight();
	std::tuple<int, int> getMinDimensions();
	void getMinDimensions(int &width, int &height);
	bool isResizable();
	void setResizable(bool toggle);
};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_WINDOW
