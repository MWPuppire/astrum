#ifndef INCLUDE_ASTRUM_WINDOW
#define INCLUDE_ASTRUM_WINDOW

#include <tuple>
#include <string>

#include "constants.hpp"

namespace Astrum {

namespace window {

	void setFullscreen(bool fullscreen);
	void setSize(int width, int height);
	void setTitle(std::string title);
	bool getFullscreen();
	std::string getTitle();
	int getWidth();
	int getHeight();
	std::tuple<int, int> getDimensions();
	std::tuple<int, int> getDesktopDimensions();
	void setPosition(int x, int y);
	std::tuple<int, int> getPosition();
	int getMinWidth();
	int getMinHeight();
	std::tuple<int, int> getMinDimensions();
	bool isResizable();
	void setResizable(bool toggle);
};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_WINDOW
