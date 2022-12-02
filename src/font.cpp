#include <string>
#include <cstdarg>
#include <cstdio>
#include <tuple>
#include <memory>
#include <cstddef>
#include <stdexcept>

#include "sdl.hpp"
#include "internals.hpp"
#include "astrum/constants.hpp"
#include "astrum/font.hpp"
#include "astrum/util.hpp"
#include "astrum/image.hpp"
#include "astrum/graphics.hpp"

#ifndef NO_DEFAULT_FONT
#	include "vera_ttf.h"
#endif

namespace Astrum {

std::shared_ptr<FontData> fontDataFromRW(SDL_RWops *rw, int size, Color color,
	int style, TextAlign align) {
	if (rw == nullptr) {
		return std::make_shared<FontData>(nullptr, color, align);
	} else {
		TTF_Font *font = TTF_OpenFontRW(rw, 1, size);
		TTF_SetFontStyle(font, style & ~Font::OUTLINE);
		if (style & Font::OUTLINE)
			TTF_SetFontOutline(font, 1);
		return std::make_shared<FontData>(font, color, align);
	}
}

Font::Font(std::shared_ptr<FontData> data) {
	this->data = data;
}

#ifndef NO_DEFAULT_FONT
Font::Font(int size, Color color, int style, TextAlign align) {
	SDL_RWops *rw = SDL_RWFromConstMem(vera_ttf, vera_ttf_len);
	this->data = fontDataFromRW(rw, size, color, style, align);
}
#endif

Font::Font(std::string path, int size, Color color, int style, TextAlign align) {
	TTF_Font *font = TTF_OpenFont(path.c_str(), size);
	if (font == nullptr) {
		this->data = std::make_shared<FontData>(nullptr, color, align);
		return;
	}
	TTF_SetFontStyle(font, style & ~OUTLINE);
	if (style & OUTLINE)
		TTF_SetFontOutline(font, 1);
	this->data = std::make_shared<FontData>(font, color, align);
}
Font::Font(std::filesystem::path path, int size, Color color, int style,
	TextAlign align) : Font(path.string(), size, color, style, align) { };
Font::Font(const unsigned char *buf, std::size_t bufLen, int size, Color color,
	int style, TextAlign align) {
	SDL_RWops *rw = SDL_RWFromConstMem(buf, bufLen);
	this->data = fontDataFromRW(rw, size, color, style, align);
}

std::shared_ptr<FontData> Font::getData() {
	return this->data;
}

Image Font::renderText(std::string text) {
	return this->renderText(text, this->data->defaultColor);
}
Image Font::renderText(std::string text, Color color) {
	if (this->data->font == nullptr) {
		throw std::runtime_error("invalid font");
	}
	SDL_Color scol = { color.r, color.g, color.b, color.a };
	SDL_Surface *surf = TTF_RenderUTF8_Solid(this->data->font, text.c_str(),
		scol);
	auto data = std::make_shared<ImageData>(surf);
	return Image(data);
}

std::tuple<int, int> Font::textSize(std::string text) {
	if (this->data->font == nullptr) {
		return std::make_tuple(0, 0);
	}
	int width, height;
	int out = TTF_SizeUTF8(this->data->font, text.c_str(), &width, &height);
	if (out != 0) {
		return std::make_tuple(0, 0);
	}
	return std::make_tuple(width, height);
}

Color Font::getColor() {
	return this->data->defaultColor;
}

void Font::setColor(Color col) {
	this->data->defaultColor = col;
}

TextAlign Font::getAlign() {
	return this->data->defaultAlign;
}

void Font::setAlign(TextAlign align) {
	this->data->defaultAlign = align;
}

void Font::close(Font &font) {
	// release resources associated with `font`
	font.data = std::shared_ptr<FontData>();
}

}; // namespace Astrum
