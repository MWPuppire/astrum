extern "C" {
	#define SDL_MAIN_HANDLED
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
}

#include "astrum/constants.hpp"
#include "astrum/image.hpp"

namespace Astrum
{

Image::Image(const char *filename)
{
	this->image = IMG_Load(filename);
	this->rw = nullptr;
}
Image::Image(std::string filename)
{
	this->image = IMG_Load(filename.c_str());
	this->rw = nullptr;
}
Image::Image(SDL_RWops *rw, const char *type)
{
	this->image = *type == '\0'
		? IMG_Load_RW(rw, 0)
		: IMG_LoadTyped_RW(rw, 0, type);
	this->rw = rw;
}
Image::Image(SDL_RWops *rw, std::string type)
{
	this->image = type == ""
		? IMG_Load_RW(rw, 0)
		: IMG_LoadTyped_RW(rw, 0, type.c_str());
	this->rw = rw;
}
Image::Image(const unsigned char *buf, int bufLen, const char *type)
{
	SDL_RWops *rw = SDL_RWFromConstMem(buf, bufLen);
	if (rw == nullptr) {
		this->image = nullptr;
		this->rw = nullptr;
	} else {
		this->image = *type == '\0'
			? IMG_Load_RW(rw, 1)
			: IMG_LoadTyped_RW(rw, 1, type);
		this->rw = rw;
	}
}
Image::Image(const unsigned char *buf, int bufLen, std::string type)
{
	SDL_RWops *rw = SDL_RWFromConstMem(buf, bufLen);
	if (rw == nullptr) {
		this->image = nullptr;
		this->rw = nullptr;
	} else {
		this->image = type == ""
			? IMG_Load_RW(rw, 1)
			: IMG_LoadTyped_RW(rw, 1, type.c_str());
		this->rw = rw;
	}
}
Image::Image(SDL_Surface *surf)
{
	this->image = surf;
	this->rw = nullptr;
}

Image::~Image()
{
	if (this->image != nullptr)
		SDL_FreeSurface(this->image);
	if (this->rw != nullptr)
		SDL_RWclose(this->rw);
}

SDL_Surface *Image::getImage()
{
	return this->image;
}

int Image::width()
{
	return this->image->w;
}

int Image::height()
{
	return this->image->h;
}

}
