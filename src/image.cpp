#include <cstddef>
#include <memory>
#include <string>
#include <filesystem>
#include <tuple>

#include "sdl.hpp"
#include "internals.hpp"
#include "astrum/constants.hpp"
#include "astrum/image.hpp"

namespace Astrum
{

Image::Image(Image &src)
{
	this->data = new ImageData(*(src.data));
}
Image::Image(ImageData &data)
{
	this->data = new ImageData(data);
}
Image::Image(std::string filename)
{
	this->data = new ImageData(IMG_Load(filename.c_str()));
	if (this->data->image != nullptr)
		SDL_SetSurfaceRLE(this->data->image, 1);
}
Image::Image(std::filesystem::path filename)
	: Image(filename.string()) { };
Image::Image(const unsigned char *buf, std::size_t bufLen, std::string type)
{
	SDL_RWops *rw = SDL_RWFromConstMem(buf, bufLen);
	if (rw == nullptr) {
		this->data = new ImageData(nullptr);
	} else {
		this->data = new ImageData(type == ""
			? IMG_Load_RW(rw, 1)
			: IMG_LoadTyped_RW(rw, 1, type.c_str()));
		if (this->data->image != nullptr)
			SDL_SetSurfaceRLE(this->data->image, 1);
	}
}
Image::Image(void *pixels, int width, int height)
{
	SDL_Surface *surf = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32,
		SDL_PIXELFORMAT_RGBA32);
	memcpy(surf->pixels, pixels, surf->pitch * height);
	this->data = new ImageData(surf);
	if (surf != nullptr)
		SDL_SetSurfaceRLE(this->data->image, 1);
}

Image::~Image()
{
	if (this->data->image != nullptr)
		SDL_FreeSurface(this->data->image);
	delete this->data;
}

ImageData *Image::getData()
{
	return this->data;
}

std::shared_ptr<Transforms> Image::getTransforms()
{
	return this->data->tran;
}

int Image::width()
{
	return this->data->image->w;
}

int Image::height()
{
	return this->data->image->h;
}

void Image::scale(double factor)
{
	this->data->tran->sx = this->data->tran->sy = factor;
}
void Image::scale(double sx, double sy)
{
	this->data->tran->sx = sx;
	this->data->tran->sy = sy;
}

void Image::translate(int dx, int dy)
{
	this->data->tran->dx = dx;
	this->data->tran->dy = dy;
}

void Image::rotate(double degrees)
{
	this->data->tran->degrees = degrees;
}

void Image::shear(double kx, double ky)
{
	this->data->tran->kx = kx;
	this->data->tran->ky = ky;
}

void Image::resetTransforms()
{
	this->data->tran->sx = 1.0;
	this->data->tran->sy = 1.0;
	this->data->tran->dx = 0;
	this->data->tran->dy = 0;
	this->data->tran->degrees = 0.0;
	this->data->tran->kx = 0.0;
	this->data->tran->ky = 0.0;
}

}
