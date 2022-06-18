#ifndef INCLUDE_ASTRUM_CONSTANTS
#define INCLUDE_ASTRUM_CONSTANTS

#include <cstdint>
#include <string>
#include <filesystem>

namespace Astrum {

/**
 * @brief A color class.
 *
 * A class for handling colors. Contains a single byte for red, green, blue, and
 * alpha. Can construct and convert to hex values.
 */
class Color {
public:
	std::uint8_t r, g, b, a;
	Color();
	Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 0xFF);
	Color(std::uint32_t hex, std::uint8_t a = 0xFF);
	std::uint32_t toHex() const;
};

extern const std::string VERSION;
extern const int VERSION_MAJOR;
extern const int VERSION_MINOR;
extern const int VERSION_PATCH;

extern const std::string DEFAULT_TITLE;
extern const std::string DEFAULT_ORG;
extern const int DEFAULT_WIDTH;
extern const int DEFAULT_HEIGHT;

/**
 * @brief Configuration for the game.
 *
 * A class containing every configuration option and all of the default options.
 */
class Config {
public:
	bool gammacorrect          = false;
	std::string appName        = DEFAULT_TITLE;
	std::string orgName        = DEFAULT_ORG;
	std::filesystem::path icon = "";
	int windowWidth            = DEFAULT_WIDTH;
	int windowHeight           = DEFAULT_HEIGHT;
	bool windowBorderless      = false;
	bool windowResizable       = false;
	int minWindowWidth         = 1;
	int minWindowHeight        = 1;
	bool windowFullscreen      = true;
	bool windowHeadless        = false;
	bool allowNoWindow         = false;
	bool saveDimensions        = true;
	bool scaleToSize           = false;
	// You can supply an existing SDL window through this parameter.
	// It takes a `void *` for two reasons:
	// 1. It keeps the header from including SDL (and thereby exposing all
	// of SDL to any file that includes Astrum)
	// 2. In theory, I could sometime extend this to allow, for example, an
	// OpenGL window, or something similar, instead of just an SDL window.
	void *existingWindow       = nullptr;
};

}; // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_CONSTANTS
