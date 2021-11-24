extern "C" {
	#include "SDL.h"
	#include "SDL_ttf.h"
	#include "SDL_gpu.h"
}

#include <string>
#include <cstdarg>
#include <cstdio>
#include <tuple>

#include "astrum/constants.hpp"
#include "astrum/assets/vera_font.hpp"
#include "astrum/font.hpp"
#include "astrum/util.hpp"

namespace Astrum
{

Font::Font(int size, Color color, int style)
{
	SDL_RWops *rw = SDL_RWFromConstMem(Vera_ttf, Vera_ttf_len);
	if (rw == nullptr) {
		SDL_Log("Could not load default font: %s\n", SDL_GetError());
		this->font = nullptr;
		return;
	} else {
		this->font = TTF_OpenFontRW(rw, 1, size);
	}
	TTF_SetFontStyle(this->font, style & ~OUTLINE);
	if (style & OUTLINE)
		TTF_SetFontOutline(this->font, 1);
	this->defaultColor = color;
}
Font::Font(std::string path, int size, Color color, int style)
{
	this->font = TTF_OpenFont(path.c_str(), size);
	if (this->font == nullptr)
		return;
	TTF_SetFontStyle(font, style & ~OUTLINE);
	if (style & OUTLINE)
		TTF_SetFontOutline(this->font, 1);
	this->defaultColor = color;
}
Font::Font(const char *path, int size, Color color, int style)
{
	this->font = TTF_OpenFont(path, size);
	if (this->font == nullptr)
		return;
	TTF_SetFontStyle(this->font, style & ~OUTLINE);
	if (style & OUTLINE)
		TTF_SetFontOutline(this->font, 1);
	this->defaultColor = color;
}
Font::Font(SDL_RWops *rw, int size, Color color, int style)
{
	font = TTF_OpenFontRW(rw, 0, size);
	if (font == nullptr)
		return;
	TTF_SetFontStyle(font, style & ~OUTLINE);
	if (style & OUTLINE)
		TTF_SetFontOutline(font, 1);
	defaultColor = color;
}
Font::Font(const unsigned char *buf, int bufLen, int size, Color color, int style)
{
	SDL_RWops *rw = SDL_RWFromConstMem(buf, bufLen);
	if (rw == nullptr) {
		this->font = nullptr;
		return;
	} else {
		this->font = TTF_OpenFontRW(rw, 0, size);
		SDL_RWclose(rw);
	}
	TTF_SetFontStyle(this->font, style & ~OUTLINE);
	if (style & OUTLINE)
		TTF_SetFontOutline(this->font, 1);
	this->defaultColor = color;
}

Font::~Font()
{
	if (this->font != nullptr)
		TTF_CloseFont(this->font);
}

GPU_Image *Font::renderText(const char *text)
{
	return this->renderText(text, this->defaultColor);
}
GPU_Image *Font::renderText(const char *text, Color color)
{
	if (this->font == nullptr)
		return nullptr;
	if (text == nullptr)
		return nullptr;
	SDL_Surface *surfaceMessage = TTF_RenderUTF8_Solid(this->font, text, color);
	if (surfaceMessage == nullptr)
		return nullptr;
	GPU_Image *out = GPU_CopyImageFromSurface(surfaceMessage);
	SDL_FreeSurface(surfaceMessage);
	return out;
}
GPU_Image *Font::renderText(std::string text)
{
	return this->renderText(text.c_str(), this->defaultColor);
}
GPU_Image *Font::renderText(std::string text, Color color)
{
	return this->renderText(text.c_str(), color);
}

int Font::textSize(const char *text, int *w, int *h)
{
	if (this->font == nullptr)
		return -1;
	if (text == nullptr)
		return -1;
	return TTF_SizeUTF8(this->font, text, w, h);
}
std::tuple<int, int> Font::textSize(const char *text)
{
	int w, h;
	int out = this->textSize(text, &w, &h);
	if (out != 0)
		return std::make_tuple(0, 0);
	return std::make_tuple(w, h);
}
int Font::textSize(std::string text, int *w, int *h)
{
	return this->textSize(text.c_str(), w, h);
}
std::tuple<int, int> Font::textSize(std::string text)
{
	return this->textSize(text.c_str());
}

int Font::textSizef(int *w, int *h, const char *text, ...)
{
	va_list args;
	va_start(args, text);
	std::string str = util::vstrformat(text, args);
	va_end(args);
	return this->textSize(str, w, h);
}
std::tuple<int, int> Font::textSizef(const char *text, ...)
{
	va_list args;
	va_start(args, text);
	std::string str = util::vstrformat(text, args);
	va_end(args);
	return this->textSize(str);
}
int Font::textSizef(int *w, int *h, std::string text, ...)
{
	std::va_list args;
	va_start(args, text);
	std::string str = util::vstrformat(text, args);
	va_end(args);
	return this->textSize(str, w, h);
}
std::tuple<int, int> Font::textSizef(std::string text, ...)
{
	std::va_list args;
	va_start(args, text);
	std::string str = util::vstrformat(text, args);
	va_end(args);
	return this->textSize(str);
}

void Font::setColor(Color col)
{
	this->defaultColor = col;
}

Color Font::getColor()
{
	return defaultColor;
}

}; // namespace Astrum
