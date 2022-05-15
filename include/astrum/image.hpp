#ifndef INCLUDE_ASTRUM_IMAGE
#define INCLUDE_ASTRUM_IMAGE

extern "C" {
	#define SDL_MAIN_HANDLED
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
}

#include <string>

#include "constants.hpp"

namespace Astrum {

class Image {
	SDL_Surface *image;

public:
	Image(const char *filename);
	Image(std::string filename);
	Image(SDL_RWops *rw, const char *type = "");
	Image(SDL_RWops *rw, std::string type = "");
	Image(const unsigned char *buf, int bufLen, const char *type = "");
	Image(const unsigned char *buf, int bufLen, std::string type = "");
	Image(SDL_Surface *surf);
	~Image();
	SDL_Surface *getImage();
};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_IMAGE
