extern "C" {
	#define SDL_MAIN_HANDLED
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
}

#include "astrum/constants.hpp"
#include "astrum/image.hpp"

#include <cstddef>
#include <memory>
#include <string>

namespace Astrum
{

struct ImageData {
	SDL_Surface *image = nullptr;
	SDL_RWops *rw = nullptr;
	ImageData(SDL_Surface *surf, SDL_RWops *rw)
		: image(surf), rw(rw) { }
};

std::unique_ptr<ImageData> imageDataFromRW(SDL_RWops *rw, std::string type)
{
	if (rw == nullptr) {
		return std::make_unique<ImageData>(nullptr, nullptr);
	} else {
		return std::make_unique<ImageData>(
			type == ""
				? IMG_Load_RW(rw, 1)
				: IMG_LoadTyped_RW(rw, 1, type.c_str()),
			rw);
	}
}

Image::Image(std::string filename)
{
	this->data = std::make_unique<ImageData>(IMG_Load(filename.c_str()), nullptr);
	if (this->data->image != nullptr)
		SDL_SetSurfaceRLE(this->data->image, 1);
}
Image::Image(const unsigned char *buf, std::size_t bufLen, std::string type)
{
	SDL_RWops *rw = SDL_RWFromConstMem(buf, bufLen);
	this->data = imageDataFromRW(rw, type);
	if (this->data->image != nullptr)
		SDL_SetSurfaceRLE(this->data->image, 1);
}
Image::Image(SDL_Surface *surf)
{
	this->data = std::make_unique<ImageData>(surf, nullptr);
	SDL_SetSurfaceRLE(this->data->image, 1);
}

Image::~Image()
{
	if (this->data->image != nullptr)
		SDL_FreeSurface(this->data->image);
	if (this->data->rw != nullptr)
		SDL_RWclose(this->data->rw);
}

SDL_Surface *Image::getImage()
{
	return this->data->image;
}

int Image::width()
{
	return this->data->image->w;
}

int Image::height()
{
	return this->data->image->h;
}

}
