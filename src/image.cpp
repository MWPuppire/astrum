extern "C" {
	#include "SDL.h"
	#include "SDL_image.h"
	#include "SDL_gpu.h"
}

#include "astrum/constants.hpp"
#include "astrum/image.hpp"

namespace Astrum
{

Image::Image(const char *filename)
{
	SDL_Surface *surf = IMG_Load(filename);
	this->image = GPU_CopyImageFromSurface(surf);
	SDL_FreeSurface(surf);
}
Image::Image(std::string filename)
{
	SDL_Surface *surf = IMG_Load(filename.c_str());
	this->image = GPU_CopyImageFromSurface(surf);
	SDL_FreeSurface(surf);
}
Image::Image(SDL_RWops *rw, const char *type)
{
	SDL_Surface *surf = *type == '\0'
		? IMG_Load_RW(rw, 0)
		: IMG_LoadTyped_RW(rw, 0, type);
	this->image = GPU_CopyImageFromSurface(surf);
	SDL_FreeSurface(surf);
}
Image::Image(SDL_RWops *rw, std::string type)
{
	SDL_Surface *surf = type == ""
		? IMG_Load_RW(rw, 0)
		: IMG_LoadTyped_RW(rw, 0, type.c_str());
	this->image = GPU_CopyImageFromSurface(surf);
	SDL_FreeSurface(surf);
}
Image::Image(const unsigned char *buf, int bufLen, const char *type)
{
	SDL_RWops *rw = SDL_RWFromConstMem(buf, bufLen);
	SDL_Surface *surf;
	if (rw == nullptr) {
		this->image = nullptr;
		return;
	} else {
		surf = *type == '\0'
			? IMG_Load_RW(rw, 1)
			: IMG_LoadTyped_RW(rw, 1, type);
	}
	this->image = GPU_CopyImageFromSurface(surf);
	SDL_FreeSurface(surf);
}
Image::Image(const unsigned char *buf, int bufLen, std::string type)
{
	SDL_RWops *rw = SDL_RWFromConstMem(buf, bufLen);
	SDL_Surface *surf;
	if (rw == nullptr) {
		this->image = nullptr;
		return;
	} else {
		surf = type == ""
			? IMG_Load_RW(rw, 1)
			: IMG_LoadTyped_RW(rw, 1, type.c_str());
	}
	this->image = GPU_CopyImageFromSurface(surf);
	SDL_FreeSurface(surf);
}

Image::~Image()
{
	if (this->image != nullptr)
		GPU_FreeImage(this->image);
}

}
