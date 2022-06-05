#ifndef INCLUDE_ASTRUM_IMAGE
#define INCLUDE_ASTRUM_IMAGE

extern "C" {
	#define SDL_MAIN_HANDLED
	#include <SDL2/SDL.h>
}

#include <string>
#include <cstddef>
#include <memory>

#include "constants.hpp"

namespace Astrum {

class Image {
private:
	std::unique_ptr<struct ImageData> data;

public:
	Image(std::string filename);
	Image(const unsigned char *buf, std::size_t bufLen, std::string type = "");
	Image(SDL_Surface *surf);
	~Image();

	SDL_Surface *getImage();
	int width();
	int height();
};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_IMAGE
