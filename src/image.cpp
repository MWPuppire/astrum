#include <cstddef>
#include <memory>
#include <string>
#include <filesystem>
#include <tuple>

#include "sdl.hpp"
#include "internals.hpp"
#include "astrum/constants.hpp"
#include "astrum/image.hpp"

namespace Astrum {

Image::Image(std::shared_ptr<ImageData> data) {
	this->data = data;
}
Image::Image(std::string filename) {
	SDL_Surface *surf = IMG_Load(filename.c_str());
	if (surf == nullptr)
		throw std::runtime_error("Failed to create image");
	SDL_SetSurfaceRLE(surf, 1);
	this->data = std::make_shared<ImageData>(surf);
}
Image::Image(std::filesystem::path filename)
	: Image(filename.string()) { };
Image::Image(const unsigned char *buf, std::size_t bufLen, std::string type) {
	SDL_RWops *rw = SDL_RWFromConstMem(buf, bufLen);
	if (rw == nullptr)
		throw std::runtime_error("Failed to create image");
	SDL_Surface *surf = type == ""
		? IMG_Load_RW(rw, 1)
		: IMG_LoadTyped_RW(rw, 1, type.c_str());
	if (surf == nullptr)
		throw std::runtime_error("Failed to create image");
	SDL_SetSurfaceRLE(surf, 1);
	this->data = std::make_shared<ImageData>(surf);
}
Image::Image(void *pixels, int width, int height) {
	SDL_Surface *surf = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32,
		SDL_PIXELFORMAT_RGBA32);
	if (surf == nullptr)
		throw std::runtime_error("Failed to create image");
	memcpy(surf->pixels, pixels, surf->pitch * height);
	SDL_SetSurfaceRLE(surf, 1);
	this->data = std::make_shared<ImageData>(surf);
}

const std::shared_ptr<ImageData> Image::getData() const {
	return this->data;
}
std::shared_ptr<ImageData> Image::getData() {
	return this->data;
}

const Transforms &Image::getTransforms() const {
	return this->data->tran;
}
Transforms &Image::getTransforms() {
	return this->data->tran;
}

int Image::getWidth() const {
	return this->data->image->w;
}

int Image::getHeight() const {
	return this->data->image->h;
}

void Image::scale(double factor) {
	this->data->tran.sx = this->data->tran.sy = factor;
}
void Image::scale(double sx, double sy) {
	this->data->tran.sx = sx;
	this->data->tran.sy = sy;
}

void Image::translate(int dx, int dy) {
	this->data->tran.dx = dx;
	this->data->tran.dy = dy;
}

void Image::rotate(double degrees) {
	this->data->tran.degrees = degrees;
}

void Image::shear(double kx, double ky) {
	this->data->tran.kx = kx;
	this->data->tran.ky = ky;
}

void Image::resetTransforms() {
	this->data->tran.sx = 1.0;
	this->data->tran.sy = 1.0;
	this->data->tran.dx = 0;
	this->data->tran.dy = 0;
	this->data->tran.degrees = 0.0;
	this->data->tran.kx = 0.0;
	this->data->tran.ky = 0.0;
}

}
