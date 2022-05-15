#ifndef INCLUDE_ASTRUM_CONSTANTS
#define INCLUDE_ASTRUM_CONSTANTS

namespace Astrum {

extern const char *VERSION;
#define VERSION_MAJOR (VERSION[0] - '0')
#define VERSION_MINOR (VERSION[2] - '0')
#define VERSION_PATCH (VERSION[4] - '0')

extern const char *DEFAULT_TITLE;
extern const int DEFAULT_WIDTH;
extern const int DEFAULT_HEIGHT;

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
	bool scaleToSize        = false;
};

}; // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_CONSTANTS
