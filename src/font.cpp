#include <string>
#include <cstdarg>
#include <cstdio>
#include <tuple>
#include <memory>
#include <cstddef>

#include "sdl.hpp"
#include "internals.hpp"
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

FontData *fontDataFromRW(SDL_RWops *rw, int size, Color color,
	int style, TextAlign align)
{
	if (rw == nullptr) {
		return new FontData(nullptr, color, align);
	} else {
		TTF_Font *font = TTF_OpenFontRW(rw, 1, size);
		TTF_SetFontStyle(font, style & ~Font::OUTLINE);
		if (style & Font::OUTLINE)
			TTF_SetFontOutline(font, 1);
		return new FontData(font, color, align);
	}
}

Font::Font(Font &src)
{
	this->data = new FontData(*(src.data));
}
Font::Font(FontData &data)
{
	this->data = new FontData(data);
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
		this->data = new FontData(nullptr, color, align);
		return;
	}
	TTF_SetFontStyle(font, style & ~OUTLINE);
	if (style & OUTLINE)
		TTF_SetFontOutline(font, 1);
	this->data = new FontData(font, color, align);
}
Font::Font(std::filesystem::path path, int size, Color color, int style,
	TextAlign align) : Font(path.string(), size, color, style, align) { };
Font::Font(const unsigned char *buf, std::size_t bufLen, int size, Color color,
	int style, TextAlign align)
{
	SDL_RWops *rw = SDL_RWFromConstMem(buf, bufLen);
	this->data = fontDataFromRW(rw, size, color, style, align);
}

Font::~Font()
{
//	if (this->data->font != nullptr)
//		TTF_CloseFont(this->data->font);
	delete this->data;
}

FontData *Font::getData()
{
	return this->data;
}

std::shared_ptr<Image> Font::renderText(std::string text)
{
	return this->renderText(text, this->data->defaultColor);
}
std::shared_ptr<Image> Font::renderText(std::string text, Color color)
{
	if (this->data->font == nullptr) {
		return nullptr;
	}
	SDL_Color scol = { color.r, color.g, color.b, color.a };
	SDL_Surface *surf = TTF_RenderUTF8_Solid(this->data->font, text.c_str(),
		scol);
	ImageData data = { surf };
	return std::make_shared<Image>(data);
}

int Font::textSize(std::string text, int &w, int &h)
{
	if (this->data->font == nullptr) {
		return -1;
	}
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
