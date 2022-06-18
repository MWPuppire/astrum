extern "C" {
	#define SDL_MAIN_HANDLED
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL2_gfxPrimitives.h>
}

#include <cstdio>
#include <vector>
#include <assert.h>
#include <cstdint>
#include <string>

#include "astrum/constants.hpp"
#include "astrum/window.hpp"
#include "astrum/graphics.hpp"
#include "astrum/font.hpp"
#include "astrum/astrum.hpp"
#include "astrum/util.hpp"
#include "internals.hpp"

namespace Astrum
{

Color::Color()
	: r(0), g(0), b(0), a(0xFF) { }
Color::Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
	: r(r), g(g), b(b), a(a) { }
Color::Color(std::uint32_t hex, std::uint8_t a)
	: r((hex >> 16) & 0xFF), g((hex >> 8) & 0xFF), b(hex & 0xFF), a(a) { }
std::uint32_t Color::toHex() const
{
	return ((unsigned) r << 16) | ((unsigned) g << 8) | (unsigned) b;
}

namespace graphics
{
	namespace
	{
		SDL_Renderer *renderer;
		void *glcontext;
		std::shared_ptr<Font> defaultFont;

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

		backgroundColor = Color(0, 0, 0, 0xFF);
		currentColor = Color(0, 0, 0, 0xFF);
		lineThickness = 0;

#ifndef NO_DEFAULT_FONT
		defaultFont = std::make_shared<Font>();
#else
		defaultFont = nullptr;
#endif

		auto drawevent = []() { drawframe(); };

		ondraw(drawevent);

		if (window::window == nullptr) {
			renderer = nullptr;
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

	void print(std::string str, int x, int y)
	{
		print(str, x, y, defaultFont, currentColor);
	}
	void print(std::string str, int x, int y, std::shared_ptr<Font> font)
	{
		print(str, x, y, font, currentColor);
	}
	void print(std::string str, int x, int y, Color col)
	{
		print(str, x, y, defaultFont, col);
	}
	void print(std::string str, int x, int y, std::shared_ptr<Font> font, Color col)
	{
		std::shared_ptr<Image> image = font->renderText(str, col);
		if (image == nullptr)
			return;

		TextAlign align = font->align();
		int textWidth, textHeight;
		font->textSize(str, textWidth, textHeight);
		int offset = align * (textWidth / 2);

		render(image, x - offset, y);
	}

	std::shared_ptr<Font> getFont()
	{
		return defaultFont;
	}

	void setFont(std::shared_ptr<Font> newFont)
	{
		defaultFont = newFont;
	}

	void render(std::shared_ptr<Image> image, int x, int y)
	{
		std::shared_ptr<Transforms> tran = image->getTransforms();
		ImageData *data = image->getData();
		SDL_Surface *surf = data->image;
		SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
		SDL_Rect sourceRect, renderRect;
		SDL_RendererFlip flip;
		double degrees;

		if (tran != nullptr) {
			sourceRect = { .x = tran->dx, .y = tran->dy,
				.w = surf->w, .h = surf->h };
			renderRect = { .x = x, .y = y,
				.w = static_cast<int>(surf->w * tran->sx),
				.h = static_cast<int>(surf->h * tran->sy) };
			flip = static_cast<SDL_RendererFlip>(0);
			degrees = tran->degrees;
		} else {
			sourceRect = { .x = 0, .y = 0, .w = surf->w, .h = surf->h };
			renderRect = { .x = x, .y = y, .w = surf->w, .h = surf->h };
			flip = static_cast<SDL_RendererFlip>(0);
			degrees = 0;
		}

		SDL_RenderCopyEx(renderer, tex, &sourceRect, &renderRect,
			degrees, nullptr, flip);
		SDL_DestroyTexture(tex);
	}

	void getVirtualCoords(int x, int y, int &virtX, int &virtY)
	{
		float logicalX, logicalY;
		SDL_RenderWindowToLogical(renderer, x, y, &logicalX, &logicalY);
		virtX = (int) logicalX;
		virtY = (int) logicalY;
	}

	std::shared_ptr<Image> screenshot()
	{
		int w, h;
		SDL_GetRendererOutputSize(renderer, &w, &h);
		SDL_Surface *sshot = SDL_CreateRGBSurface(0, w, h, 32,
			0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
		SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_ARGB8888,
			sshot->pixels, sshot->pitch);
		ImageData data = ImageData(sshot);
		return std::make_shared<Image>(data);
	}
};

}; // namespace Astrum
