extern "C" {
	#include "SDL.h"
	#include "SDL_ttf.h"
	#include "SDL_gpu.h"
}

#include <string>

#include "astrum/constants.hpp"
#include "astrum/assets/vera_font.hpp"
#include "astrum/font.hpp"

namespace Astrum
{

Font::Font(int size, Color color, int style)
{
	SDL_RWops *rw = SDL_RWFromConstMem(_Vera_ttf, _Vera_ttf_len);
	if (rw == NULL) {
		SDL_Log("Could not load default font: %s\n", SDL_GetError());
		font = NULL;
	} else {
		font = TTF_OpenFontRW(rw, 0, size);
		SDL_RWclose(rw);
	}
	if (font == NULL)
		return;
	TTF_SetFontStyle(font, style & ~OUTLINE);
	if (style & OUTLINE)
		TTF_SetFontOutline(font, 1);
	defaultColor = color;
}
Font::Font(std::string path, int size, Color color, int style)
{
	font = TTF_OpenFont(path.c_str(), size);
	if (font == NULL)
		return;
	TTF_SetFontStyle(font, style & ~OUTLINE);
	if (style & OUTLINE)
		TTF_SetFontOutline(font, 1);
	defaultColor = color;
}
Font::Font(const char *path, int size, Color color, int style)
{
	font = TTF_OpenFont(path, size);
	if (font == NULL)
		return;
	TTF_SetFontStyle(font, style & ~OUTLINE);
	if (style & OUTLINE)
		TTF_SetFontOutline(font, 1);
	defaultColor = color;
}
Font::Font(SDL_RWops *rw, int size, Color color, int style)
{
	font = TTF_OpenFontRW(rw, 0, size);
	if (font == NULL)
		return;
	TTF_SetFontStyle(font, style & ~OUTLINE);
	if (style & OUTLINE)
		TTF_SetFontOutline(font, 1);
	defaultColor = color;
}

Font::~Font()
{
	TTF_CloseFont(font);
}

GPU_Image *Font::renderText(const char *text)
{
	return renderText(text, defaultColor);
}
GPU_Image *Font::renderText(const char *text, Color color)
{
	if (font == NULL)
		return NULL;
	SDL_Surface *surfaceMessage = TTF_RenderUTF8_Solid(font, text, color);
	if (surfaceMessage == NULL)
		return NULL;
	GPU_Image *out = GPU_CopyImageFromSurface(surfaceMessage);
	SDL_FreeSurface(surfaceMessage);
	return out;
}
GPU_Image *Font::renderText(std::string text)
{
	return renderText(text.c_str(), defaultColor);
}
GPU_Image *Font::renderText(std::string text, Color color)
{
	return renderText(text.c_str(), color);
}

int Font::textSize(const char *text, int *w, int *h)
{
	return TTF_SizeUTF8(font, text, w, h);
}
int Font::textSize(std::string text, int *w, int *h)
{
	return textSize(text.c_str(), w, h);
}

void Font::setColor(Color col)
{
	defaultColor = col;
}

Color Font::getColor()
{
	return defaultColor;
}

}; // namespace Astrum
