extern "C" {
	#define SDL_MAIN_HANDLED
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_ttf.h>
}

#include <string>
#include <cstdarg>
#include <cstdio>
#include <tuple>
#include <memory>
#include <cstddef>

#include "astrum/constants.hpp"
#include "astrum/font.hpp"
#include "astrum/util.hpp"
#include "astrum/image.hpp"
#include "astrum/graphics.hpp"

#ifndef NO_DEFAULT_FONT
#include "astrum/assets/vera_font.hpp"
#endif

namespace Astrum
{

struct FontData {
	TTF_Font *font = nullptr;
	SDL_RWops *rw = nullptr;
	Color defaultColor;
	TextAlign defaultAlign;
	FontData(TTF_Font *font, SDL_RWops *rw, Color defaultColor, TextAlign defaultAlign)
		: font(font), rw(rw), defaultColor(defaultColor), defaultAlign(defaultAlign) { }
};

std::unique_ptr<FontData> fontDataFromRW(SDL_RWops *rw, int size, Color color, int style, TextAlign align)
{
	if (rw == nullptr) {
		return std::make_unique<FontData>(nullptr, nullptr, color, align);
	} else {
		TTF_Font *font = TTF_OpenFontRW(rw, 1, size);
		TTF_SetFontStyle(font, style & ~Font::OUTLINE);
		if (style & Font::OUTLINE)
			TTF_SetFontOutline(font, 1);
		return std::make_unique<FontData>(font, rw, color, align);
	}
}

#ifndef NO_DEFAULT_FONT
Font::Font(int size, Color color, int style, TextAlign align)
{
	SDL_RWops *rw = SDL_RWFromConstMem(Vera_ttf, Vera_ttf_len);
	this->data = fontDataFromRW(rw, size, color, style, align);
}
#endif

Font::Font(std::string path, int size, Color color, int style, TextAlign align)
{
	TTF_Font *font = TTF_OpenFont(path.c_str(), size);
	if (font == nullptr) {
		this->data = std::make_unique<FontData>(nullptr, nullptr, color, align);
		return;
	}
	TTF_SetFontStyle(font, style & ~OUTLINE);
	if (style & OUTLINE)
		TTF_SetFontOutline(font, 1);
	this->data = std::make_unique<FontData>(font, nullptr, color, align);
}
Font::Font(const unsigned char *buf, std::size_t bufLen, int size, Color color, int style, TextAlign align)
{
	SDL_RWops *rw = SDL_RWFromConstMem(buf, bufLen);
	this->data = fontDataFromRW(rw, size, color, style, align);
}

Font::~Font()
{
	if (this->data->font != nullptr)
		TTF_CloseFont(this->data->font);
	if (this->data->rw != nullptr)
		SDL_RWclose(this->data->rw);
}

Image *Font::renderText(std::string text)
{
	return this->renderText(text, this->data->defaultColor);
}
Image *Font::renderText(std::string text, Color color)
{
	if (this->data->font == nullptr)
		return nullptr;
	SDL_Color scol = { color.r, color.g, color.b, color.a };
	SDL_Surface *surf = TTF_RenderUTF8_Solid(this->data->font, text.c_str(), scol);
	return new Image(surf);
}

int Font::textSize(std::string text, int &w, int &h)
{
	if (this->data->font == nullptr)
		return -1;
	int width, height;
	int out = TTF_SizeUTF8(this->data->font, text.c_str(), &width, &height);
	w = width;
	h = height;
	return out;
}
std::tuple<int, int> Font::textSize(std::string text)
{
	int w, h;
	int out = this->textSize(text, w, h);
	if (out != 0)
		return std::make_tuple(0, 0);
	return std::make_tuple(w, h);
}

int Font::textSizef(int &w, int &h, std::string text, ...)
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

Color Font::color()
{
	return this->data->defaultColor;
}

void Font::setColor(Color col)
{
	this->data->defaultColor = col;
}

TextAlign Font::align()
{
	return this->data->defaultAlign;
}

void Font::setAlign(TextAlign align)
{
	this->data->defaultAlign = align;
}

}; // namespace Astrum
