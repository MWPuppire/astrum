extern "C" {
	#define SDL_MAIN_HANDLED
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL_gpu.h>
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
	namespace
	{
//		SDL_Renderer *renderer;
		GPU_Target *screen;
		void *glcontext;
		Font *defaultFont;

		Color backgroundColor;
		Color currentColor;

		Config *settings;
	};

	void drawframe()
	{
		GPU_Flip(screen);
		GPU_ClearColor(screen, backgroundColor);
	}

	int InitGraphics(Config *conf)
	{
		settings = conf;

		if (window::window != nullptr)
			glcontext = SDL_GL_CreateContext(window::window);
		else
			glcontext = nullptr;

		backgroundColor = color(0, 0, 0, 0xFF);
		currentColor = color(0, 0, 0, 0xFF);

#ifndef NO_DEFAULT_FONT
		defaultFont = new Font();
#else
		defaultFont = nullptr;
#endif

		auto drawevent = []() { drawframe(); };

		ondraw(drawevent);

		if (window::window == nullptr) {
			screen = GPU_Init(0, 0, GPU_DEFAULT_INIT_FLAGS);
//			renderer = NULL;
		} else {
			GPU_SetInitWindow(SDL_GetWindowID(window::window));
			screen = GPU_Init(window::getWidth(), window::getHeight(), GPU_DEFAULT_INIT_FLAGS);
//			renderer = SDL_CreateRenderer(window::window, -1, 0);
		}

		if (conf->scaleToSize) {
//			SDL_RenderSetLogicalSize(renderer, conf->windowWidth, conf->windowHeight);
//			SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
			GPU_SetVirtualResolution(screen, conf->windowWidth, conf->windowHeight);
		}

		auto resizeevent = [](int w, int h)
		{
			GPU_SetWindowResolution(w, h);
			if (settings->scaleToSize)
				GPU_SetVirtualResolution(screen, settings->windowWidth, settings->windowHeight);
		};
		onresize(resizeevent);

		return 0;
	}

	void QuitGraphics()
	{
		GPU_FreeTarget(screen);
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
		print(str, x, y, defaultFont, currentColor);
	}
	void print(const char *str, float x, float y, Font *font)
	{
		print(str, x, y, font, currentColor);
	}
	void print(const char *str, float x, float y, Color col)
	{
		print(str, x, y, defaultFont, col);
	}
	void print(const char *str, float x, float y, Font *font, Color col)
	{
		Image *image = font->renderText(str, col);
		if (image == nullptr)
			return;
		int offsetX, offsetY;
		font->textSize(str, &offsetX, &offsetY);
		x += 0.5 * (float) offsetX;
		y += 0.5 * (float) offsetY;
		render(image, x, y);
	}

	void printf(float x, float y, const char *str, ...)
	{
		va_list args;
		va_start(args, str);
		std::string formatted = util::vstrformat(str, args);
		va_end(args);
		print(formatted.c_str(), x, y, defaultFont, currentColor);
	}
	void printf(float x, float y, Font *font, const char *str, ...)
	{
		va_list args;
		va_start(args, str);
		std::string formatted = util::vstrformat(str, args);
		va_end(args);
		print(formatted.c_str(), x, y, font, currentColor);
	}
	void printf(float x, float y, Color col, const char *str, ...)
	{
		va_list args;
		va_start(args, str);
		std::string formatted = util::vstrformat(str, args);
		va_end(args);
		print(formatted.c_str(), x, y, defaultFont, col);
	}
	void printf(float x, float y, Font *font, Color col, const char *str, ...)
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

	void render(Image *image, float x, float y)
	{
		SDL_Surface *surf = image->getImage();
		GPU_Image *blit = GPU_CopyImageFromSurface(surf);
		GPU_Blit(blit, nullptr, screen, x, y);
		GPU_FreeImage(blit);
	}

	void getVirtualCoords(int x, int y, int *virtX, int *virtY)
	{
		float modifiedX, modifiedY;
		GPU_GetVirtualCoords(screen, &modifiedX, &modifiedY, x, y);
		*virtX = (int) modifiedX;
		*virtY = (int) modifiedY;
	}
};

}; // namespace Astrum
