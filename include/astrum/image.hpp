#ifndef INCLUDE_ASTRUM_IMAGE
#define INCLUDE_ASTRUM_IMAGE

extern "C" {
	#include "SDL.h"
	#include "SDL_image.h"
	#include "SDL_gpu.h"
}

#include <string>

#include "constants.hpp"

namespace Astrum {

class Image {
	GPU_Image *image;

public:
	Image(const char *filename);
	Image(std::string filename);
	Image(SDL_RWops *rw, const char *type = "");
	Image(SDL_RWops *rw, std::string type = "");
	Image(const unsigned char *buf, int bufLen, const char *type = "");
	Image(const unsigned char *buf, int bufLen, std::string type = "");
	~Image();
};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_IMAGE
