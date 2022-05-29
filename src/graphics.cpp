extern "C" {
	#define SDL_MAIN_HANDLED
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL2_gfxPrimitives.h>
}

#include <cstdio>
#include <vector>
#include <assert.h>

#include "astrum/constants.hpp"
#include "astrum/window.hpp"
#include "astrum/graphics.hpp"
#include "astrum/font.hpp"
#include "astrum/astrum.hpp"
#include "astrum/util.hpp"

namespace Astrum
{

Color color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return (Color) { .r = r, .g = g, .b = b, .a = a };
}

namespace graphics
{
	namespace
	{
		SDL_Renderer *renderer;
		void *glcontext;
		Font *defaultFont;

		Color backgroundColor;
		Color currentColor;
		int lineThickness;
	};

	void drawframe()
	{
		Color col = backgroundColor;
		SDL_RenderPresent(renderer);
		SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
		SDL_RenderClear(renderer);
	}

	int InitGraphics(Config &conf)
	{
		if (window::window != nullptr)
			glcontext = SDL_GL_CreateContext(window::window);
		else
			glcontext = nullptr;

		backgroundColor = color(0, 0, 0, 0xFF);
		currentColor = color(0, 0, 0, 0xFF);
		lineThickness = 0;

#ifndef NO_DEFAULT_FONT
		defaultFont = new Font();
#else
		defaultFont = nullptr;
#endif

		auto drawevent = []() { drawframe(); };

		ondraw(drawevent);

		if (window::window == nullptr) {
			renderer = NULL;
		} else {
			if (conf.existingWindow != nullptr)
				renderer = SDL_GetRenderer(window::window);
			if (renderer == nullptr)
				renderer = SDL_CreateRenderer(window::window, -1, 0);
		}

		if (conf.scaleToSize) {
			SDL_RenderSetLogicalSize(renderer, conf.windowWidth, conf.windowHeight);
			SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
		}

		return 0;
	}

	void QuitGraphics()
	{
		SDL_GL_DeleteContext(glcontext);
		SDL_DestroyRenderer(renderer);
		IMG_Quit();
	}

	Color getBackgroundColor()
	{
		return backgroundColor;
	}

	void setBackgroundColor(Color color)
	{
		backgroundColor = color;
	}

	void setLineThickness(int thickness)
	{
		lineThickness = thickness;
	}

	int getLineThickness()
	{
		return lineThickness;
	}

	Color getColor()
	{
		return currentColor;
	}

	void setColor(Color color)
	{
		currentColor = color;
	}

	void rectangle(int x, int y, int width, int height, bool filled)
	{
		rectangle(x, y, width, height, currentColor, filled);
	}
	void rectangle(int x, int y, int width, int height, Color col, bool filled)
	{
		if (filled)
			boxRGBA(renderer, x, y, x + width, y + height, col.r,
				col.g, col.b, col.a);
		else
			rectangleRGBA(renderer, x, y, x + width, y + height,
				col.r, col.g, col.b, col.a);
	}

	void rectangleFilled(int x, int y, int width, int height)
	{
		rectangle(x, y, width, height, currentColor, true);
	}
	void rectangleFilled(int x, int y, int width, int height, Color col)
	{
		rectangle(x, y, width, height, col, true);
	}

	void circle(int x, int y, int radius, bool filled)
	{
		circle(x, y, radius, currentColor, filled);
	}
	void circle(int x, int y, int radius, Color col, bool filled)
	{
		if (filled)
			filledCircleRGBA(renderer, x, y, radius, col.r, col.g,
				col.b, col.a);
		else
			circleRGBA(renderer, x, y, radius, col.r, col.g, col.b,
				col.a);
	}

	void circleFilled(int x, int y, int radius)
	{
		circle(x, y, radius ,currentColor, true);
	}
	void circleFilled(int x, int y, int radius, Color col)
	{
		circle(x, y, radius, col, true);
	}

	void triangle(int x1, int y1, int x2, int y2, int x3, int y3, bool filled)
	{
		triangle(x1, y1, x2, y2, x3, y3, currentColor, filled);
	}
	void triangle(int x1, int y1, int x2, int y2, int x3, int y3, Color col, bool filled)
	{
		if (filled)
			filledTrigonRGBA(renderer, x1, y1, x2, y2, x3, y3,
				col.r, col.g, col.b, col.a);
		else
			trigonRGBA(renderer, x1, y1, x2, y2, x3, y3, col.r,
				col.g, col.b, col.a);
	}

	void triangleFilled(int x1, int y1, int x2, int y2, int x3, int y3)
	{
		triangle(x1, y1, x2, y2, x3, y3, currentColor, true);
	}
	void triangleFilled(int x1, int y1, int x2, int y2, int x3, int y3, Color col)
	{
		triangle(x1, y1, x2, y2, x3, y3, col, true);
	}

	void ellipse(int x, int y, int rx, int ry, bool filled)
	{
		ellipse(x, y, rx, ry, currentColor, filled);
	}
	void ellipse(int x, int y, int rx, int ry, Color col, bool filled)
	{
		if (filled)
			filledEllipseRGBA(renderer, x, y, rx, ry, col.r, col.g,
				col.b, col.a);
		else
			ellipseRGBA(renderer, x, y, rx, ry, col.r, col.g, col.b,
				col.a);
	}

	void ellipseFilled(int x, int y, int rx, int ry)
	{
		ellipse(x, y, rx, ry, currentColor, true);
	}
	void ellipseFilled(int x, int y, int rx, int ry, Color col)
	{
		ellipse(x, y, rx, ry, col, true);
	}

	void polygon(std::vector<int> vertices, bool filled)
	{
		polygon(vertices, currentColor, filled);
	}
	void polygon(std::vector<int> vertices, Color col, bool filled)
	{
		assert((vertices.size() & 1) == 0);
		size_t len = vertices.size() / 2;
		short x[len];
		short y[len];
		for (size_t i = 0; i < len; i++) {
			x[i] = vertices[i * 2];
			y[i] = vertices[i * 2 + 1];
		}
		if (filled)
			filledPolygonRGBA(renderer, x, y, len, col.r, col.g,
				col.b, col.a);
		else
			polygonRGBA(renderer, x, y, len, col.r, col.g, col.b,
				col.a);
	}

	void polygonFilled(std::vector<int> vertices)
	{
		polygon(vertices, currentColor, true);
	}
	void polygonFilled(std::vector<int> vertices, Color col)
	{
		polygon(vertices, col, true);
	}

	void point(int x, int y)
	{
		point(x, y, currentColor);
	}
	void point(int x, int y, Color col)
	{
		pixelRGBA(renderer, x, y, col.r, col.g, col.b, col.a);
	}

	void line(int x1, int y1, int x2, int y2)
	{
		line(x1, y1, x2, y2, currentColor);
	}
	void line(int x1, int y1, int x2, int y2, Color col)
	{
		if (lineThickness > 1)
			thickLineRGBA(renderer, x1, y1, x2, y2, lineThickness,
				col.r, col.g, col.b, col.a);
		else
			lineRGBA(renderer, x1, y1, x2, y2, col.r, col.g, col.b,
				col.a);
	}
	void line(std::vector<int> lines)
	{
		assert((lines.size() & 3) == 0);
		for (size_t i = 0; i < lines.size(); i += 4)
			line(lines[i], lines[i + 1], lines[i + 2], lines[i + 3],
				currentColor);
	}
	void line(std::vector<int> lines, Color col)
	{
		assert((lines.size() & 3) == 0);
		for (size_t i = 0; i < lines.size(); i += 4)
			line(lines[i], lines[i + 1], lines[i + 2], lines[i + 3],
				col);
	}

	void arc(int x, int y, int r, int a1, int a2, bool filled)
	{
		arc(x, y, r, a1, a2, currentColor, filled);
	}
	void arc(int x, int y, int r, int a1, int a2, Color col, bool filled)
	{
		if (filled)
			filledPieRGBA(renderer, x, y, r, a1, a2, col.r, col.g,
				col.b, col.a);
		else
			arcRGBA(renderer, x, y, r, a1, a2, col.r, col.g, col.b,
				col.a);
	}
	void arcFilled(int x, int y, int r, int a1, int a2)
	{
		arc(x, y, r, a1, a2, currentColor, true);
	}
	void arcFilled(int x, int y, int r, int a1, int a2, Color col)
	{
		arc(x, y, r, a1, a2, col, true);
	}

	void clear()
	{
		Color col = backgroundColor;
		SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
		SDL_RenderClear(renderer);
	}
	void clear(Color col)
	{
		SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
		SDL_RenderClear(renderer);
	}

	void print(const char *str, int x, int y)
	{
		print(str, x, y, defaultFont, currentColor);
	}
	void print(const char *str, int x, int y, Font *font)
	{
		print(str, x, y, font, currentColor);
	}
	void print(const char *str, int x, int y, Color col)
	{
		print(str, x, y, defaultFont, col);
	}
	void print(const char *str, int x, int y, Font *font, Color col)
	{
		Image *image = font->renderText(str, col);
		if (image == nullptr)
			return;
		render(image, x, y);
		delete image;
	}

	void printf(int x, int y, const char *str, ...)
	{
		va_list args;
		va_start(args, str);
		std::string formatted = util::vstrformat(str, args);
		va_end(args);
		print(formatted.c_str(), x, y, defaultFont, currentColor);
	}
	void printf(int x, int y, Font *font, const char *str, ...)
	{
		va_list args;
		va_start(args, str);
		std::string formatted = util::vstrformat(str, args);
		va_end(args);
		print(formatted.c_str(), x, y, font, currentColor);
	}
	void printf(int x, int y, Color col, const char *str, ...)
	{
		va_list args;
		va_start(args, str);
		std::string formatted = util::vstrformat(str, args);
		va_end(args);
		print(formatted.c_str(), x, y, defaultFont, col);
	}
	void printf(int x, int y, Font *font, Color col, const char *str, ...)
	{
		va_list args;
		va_start(args, str);
		std::string formatted = util::vstrformat(str, args);
		va_end(args);
		print(formatted.c_str(), x, y, font, col);
	}

	Font *getFont()
	{
		return defaultFont;
	}

	void setFont(Font *newFont)
	{
		defaultFont = newFont;
	}

// hackish fix
// There is apparently a bug with rendering images in Emscripten SDL
// https://github.com/emscripten-core/emscripten/issues/16223
// This is a solution to display images without using SDL textures, which allows
// the code to display properly.
// Since this is almost certainly worse than using textures, only use the custom
// `render` function with Emscripten; otherwise, use normal SDL code.
#ifdef __EMSCRIPTEN__
	void render(Image *image, int x, int y)
	{
		SDL_Surface *surf = image->getImage();
		int w = surf->w, h = surf->h;
		const SDL_PixelFormat *format = surf->format;
		const int byteSize = format->BytesPerPixel;
		const int yMultiplier = surf->pitch / byteSize;

		Uint32 colorKey;
		int hasKey = SDL_GetColorKey(surf, &colorKey);

		SDL_LockSurface(surf);
		const void *pixels = surf->pixels;
		for (int surfX = 0; surfX < w; surfX++) {
			for (int surfY = 0; surfY < h; surfY++) {
				int idx = surfX + surfY * yMultiplier;
				Uint32 pixel;
				switch (byteSize) {
				case 1:
					pixel = ((Uint8 *) pixels)[idx];
					break;
				case 2:
					pixel = ((Uint16 *) pixels)[idx];
					break;
				case 3: {
					struct pixelTriplet { Uint8 a, b, c; };
					union { pixelTriplet trip; Uint32 u32; } x;
					x.trip = ((pixelTriplet *) pixels)[idx];
					pixel = x.u32;
					break;
				}
				case 4:
					pixel = ((Uint32 *) pixels)[idx];
					break;
				}

				Uint8 r, g, b, a;
				SDL_GetRGBA(pixel, format, &r, &g, &b, &a);
				if (hasKey != 0 || pixel != colorKey)
					pixelRGBA(renderer, x + surfX, y + surfY, r, g, b, a);
			}
		}
		SDL_UnlockSurface(surf);
	}
#else
	void render(Image *image, int x, int y)
	{
		SDL_Surface *surf = image->getImage();
		SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
		SDL_Rect renderRect = { .x = x, .y = y, .w = surf->w, .h = surf->h };
		SDL_RenderCopy(renderer, tex, NULL, &renderRect);
		SDL_DestroyTexture(tex);
	}
#endif

	void getVirtualCoords(int x, int y, int &virtX, int &virtY)
	{
		float logicalX, logicalY;
		SDL_RenderWindowToLogical(renderer, x, y, &logicalX, &logicalY);
		virtX = (int) logicalX;
		virtY = (int) logicalY;
	}

	Image *screenshot()
	{
		int w, h;
		SDL_GetRendererOutputSize(renderer, &w, &h);
		SDL_Surface *sshot = SDL_CreateRGBSurface(0, w, h, 32,
			0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
		SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888,
			sshot->pixels, sshot->pitch);
		return new Image(sshot);
	}
};

}; // namespace Astrum
