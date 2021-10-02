#ifndef INCLUDE_ASTRUM_CONSTANTS
#define INCLUDE_ASTRUM_CONSTANTS

namespace Astrum {

extern const char *VERSION;
const int VERSION_MAJOR = 0;
const int VERSION_MINOR = 1;
const int VERSION_PATCH = 0;

extern const char *DEFAULT_TITLE;
const int DEFAULT_WIDTH = 640;
const int DEFAULT_HEIGHT = 480;

/**
 * @brief Configuration for the game.
 *
 * A class containing every configuration
 * option and all of the default options.
 */
class Config {
public:
	bool gammacorrect       = false;
	const char *windowTitle = DEFAULT_TITLE;
	const char *icon        = nullptr;
	int windowWidth         = DEFAULT_WIDTH;
	int windowHeight        = DEFAULT_HEIGHT;
	bool windowBorderless   = false;
	bool windowResizable    = false;
	int minWindowWidth      = 1;
	int minWindowHeight     = 1;
	bool windowFullscreen   = true;
	bool windowHeadless     = false;
	bool allowNoWindow      = false;
	bool saveDimensions     = true;
};

}; // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_CONSTANTS
