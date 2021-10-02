extern "C" {
	#include "SDL.h"
	#include "SDL_image.h"
	#include "SDL_gpu.h"
}

#include <cstdio>
#include <vector>

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
	GPU_Target *screen;
	void *glcontext;
	Font *font;

	namespace
	{
		Color backgroundColor;
		Color currentColor;
	};

	void drawframe()
	{
		GPU_Flip(screen);
		GPU_ClearColor(screen, backgroundColor);
	}

	int InitGraphics()
	{
		if (window::window != nullptr)
			glcontext = SDL_GL_CreateContext(window::window);
		else
			glcontext = nullptr;

		backgroundColor = color(0, 0, 0, 0xFF);
		currentColor = color(0, 0, 0, 0xFF);

		font = new Font();

		auto drawevent = []() { drawframe(); };

		ondraw(drawevent);

		if (window::window == nullptr) {
			screen = GPU_Init(0, 0, GPU_DEFAULT_INIT_FLAGS);
		} else {
			GPU_SetInitWindow(SDL_GetWindowID(window::window));
			screen = GPU_Init(window::getWidth(), window::getHeight(), GPU_DEFAULT_INIT_FLAGS);
		}

		return 0;
	}

	void QuitGraphics()
	{
		SDL_GL_DeleteContext(glcontext);
		IMG_Quit();
		GPU_Quit();
	}

	Color getBackgroundColor()
	{
		return backgroundColor;
	}

	void setBackgroundColor(Color color)
	{
		backgroundColor = color;
	}

	void setLineThickness(float thickness)
	{
		GPU_SetLineThickness(thickness);
	}

	float getLineThickness()
	{
		return GPU_GetLineThickness();
	}

	Color getColor()
	{
		return currentColor;
	}

	void setColor(Color color)
	{
		currentColor = color;
	}

	void rectangle(float x, float y, float width, float height, bool filled)
	{
		rectangle(x, y, width, height, currentColor, filled);
	}
	void rectangle(float x, float y, float width, float height, Color col, bool filled)
	{
		if (filled)
			GPU_RectangleFilled(screen, x, y, x + width, y + height, col);
		else
			GPU_Rectangle(screen, x, y, x + width, y + height, col);
	}

	void rectangleFilled(float x, float y, float width, float height)
	{
		rectangleFilled(x, y, width, height, currentColor);
	}
	void rectangleFilled(float x, float y, float width, float height, Color col)
	{
		GPU_RectangleFilled(screen, x, y, x + width, y + height, col);
	}

	void circle(float x, float y, float radius, bool filled)
	{
		circle(x, y, radius, currentColor, filled);
	}
	void circle(float x, float y, float radius, Color col, bool filled)
	{
		if (filled)
			GPU_CircleFilled(screen, x, y, radius, col);
		else
			GPU_Circle(screen, x, y, radius, col);
	}

	void circleFilled(float x, float y, float radius)
	{
		circleFilled(x, y, radius, currentColor);
	}
	void circleFilled(float x, float y, float radius, Color col)
	{
		GPU_CircleFilled(screen, x, y, radius, col);
	}

	void triangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled)
	{
		triangle(x1, y1, x2, y2, x3, y3, currentColor, filled);
	}
	void triangle(float x1, float y1, float x2, float y2, float x3, float y3, Color col, bool filled)
	{
		if (filled)
			GPU_TriFilled(screen, x1, y1, x2, y2, x3, y3, col);
		else
			GPU_Tri(screen, x1, y1, x2, y2, x3, y3, col);
	}

	void triangleFilled(float x1, float y1, float x2, float y2, float x3, float y3)
	{
		triangleFilled(x1, y1, x2, y2, x3, y3, currentColor);
	}
	void triangleFilled(float x1, float y1, float x2, float y2, float x3, float y3, Color col)
	{
		GPU_TriFilled(screen, x1, y1, x2, y2, x3, y3, col);
	}

	void ellipse(float x, float y, float rx, float ry, bool filled)
	{
		ellipse(x, y, rx, ry, 0, currentColor, filled);
	}
	void ellipse(float x, float y, float rx, float ry, float degrees, bool filled)
	{
		ellipse(x, y, rx, ry, degrees, currentColor, filled);
	}
	void ellipse(float x, float y, float rx, float ry, Color col, bool filled)
	{
		ellipse(x, y, rx, ry, 0, col, filled);
	}
	void ellipse(float x, float y, float rx, float ry, float degrees, Color col, bool filled)
	{
		if (filled)
			GPU_EllipseFilled(screen, x, y, rx, ry, degrees, col);
		else
			GPU_Ellipse(screen, x, y, rx, ry, degrees, col);
	}

	void ellipseFilled(float x, float y, float rx, float ry)
	{
		ellipse(x, y, rx, ry, 0, currentColor);
	}
	void ellipseFilled(float x, float y, float rx, float ry, float degrees)
	{
		ellipseFilled(x, y, rx, ry, degrees, currentColor);
	}
	void ellipseFilled(float x, float y, float rx, float ry, Color col)
	{
		ellipseFilled(x, y, rx, ry, 0, col);
	}
	void ellipseFilled(float x, float y, float rx, float ry, float degrees, Color col)
	{
		GPU_EllipseFilled(screen, x, y, rx, ry, degrees, col);
	}

	void polygon(std::vector<float> vertices, bool filled)
	{
		polygon(vertices, currentColor, filled);
	}
	void polygon(std::vector<float> vertices, Color col, bool filled)
	{
		if (filled)
			GPU_PolygonFilled(screen, vertices.size() / 2, vertices.data(), col);
		else
			GPU_Polygon(screen, vertices.size() / 2, vertices.data(), col);
	}

	void polygonFilled(std::vector<float> vertices)
	{
		polygonFilled(vertices, currentColor);
	}
	void polygonFilled(std::vector<float> vertices, Color col)
	{
		GPU_PolygonFilled(screen, vertices.size() / 2, vertices.data(), col);
	}

	void point(float x, float y)
	{
		point(x, y, currentColor);
	}
	void point(float x, float y, Color col)
	{
		GPU_Pixel(screen, x, y, col);
	}

	void line(float x1, float y1, float x2, float y2)
	{
		line(x1, y1, x2, y2, currentColor);
	}
	void line(float x1, float y1, float x2, float y2, Color col)
	{
		GPU_Line(screen, x1, y1, x2, y2, col);
	}
	void line(std::vector<float> lines)
	{
		line(lines, currentColor);
	}
	void line(std::vector<float> lines, Color col)
	{
		for (size_t i = 0; i < lines.size(); i += 4)
			GPU_Line(screen, lines[i], lines[i + 1], lines[i + 2], lines[i + 3], col);
	}

	void arc(float x, float y, float r, float a1, float a2, bool filled)
	{
		arc(x, y, r, a1, a2, currentColor, filled);
	}
	void arc(float x, float y, float r, float a1, float a2, Color col, bool filled)
	{
		if (filled)
			GPU_ArcFilled(screen, x, y, r, a1, a2, col);
		else
			GPU_ArcFilled(screen, x, y, r, a1, a2, col);
	}

	void arcFilled(float x, float y, float r, float a1, float a2)
	{
		arcFilled(x, y, r, a1, a2, currentColor);
	}
	void arcFilled(float x, float y, float r, float a1, float a2, Color col)
	{
		GPU_ArcFilled(screen, x, y, r, a1, a2, col);
	}

	void clear()
	{
		clear(currentColor);
	}
	void clear(Color col)
	{
		GPU_ClearColor(screen, col);
	}

	void print(const char *str, float x, float y)
	{
		print(str, x, y, font);
	}
	void print(const char *str, float x, float y, Font *font)
	{
		GPU_Image *rendered = font->renderText(str);
		if (rendered == nullptr)
			return;
		int offsetX, offsetY;
		font->textSize(str, &offsetX, &offsetY);
		x += 0.5 * (float) offsetX;
		y += 0.5 * (float) offsetY;
		GPU_Blit(rendered, nullptr, screen, x, y);
		GPU_FreeImage(rendered);
	}
	void print(const char *str, float x, float y, Color col)
	{
		print(str, x, y, font, col);
	}
	void print(const char *str, float x, float y, Font *font, Color col)
	{
		GPU_Image *rendered = font->renderText(str, col);
		if (rendered == nullptr)
			return;
		int offsetX, offsetY;
		font->textSize(str, &offsetX, &offsetY);
		x += 0.5 * (float) offsetX;
		y += 0.5 * (float) offsetY;
		GPU_Blit(rendered, nullptr, screen, x, y);
		GPU_FreeImage(rendered);
	}

	void printf(float x, float y, const char *str, ...)
	{
		va_list args;
		va_start(args, str);
		std::string formatted = util::vstrformat(str, args);
		va_end(args);
		print(formatted.c_str(), x, y);
	}
	void printf(float x, float y, Font *font, const char *str, ...)
	{
		va_list args;
		va_start(args, str);
		std::string formatted = util::vstrformat(str, args);
		va_end(args);
		print(formatted.c_str(), x, y, font);
	}
	void printf(float x, float y, Color col, const char *str, ...)
	{
		va_list args;
		va_start(args, str);
		std::string formatted = util::vstrformat(str, args);
		va_end(args);
		print(formatted.c_str(), x, y, col);
	}
	void printf(float x, float y, Font *font, Color col, const char *str, ...)
	{
		va_list args;
		va_start(args, str);
		std::string formatted = util::vstrformat(str, args);
		va_end(args);
		print(formatted.c_str(), x, y, font, col);
	}
};

}; // namespace Astrum
