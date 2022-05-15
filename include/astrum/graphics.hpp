#ifndef INCLUDE_ASTRUM_GRAPHICS
#define INCLUDE_ASTRUM_GRAPHICS

extern "C" {
	#define SDL_MAIN_HANDLED
	#include <SDL2/SDL.h>
}

#include <vector>

#include "constants.hpp"
#include "font.hpp"
#include "image.hpp"

namespace Astrum {

typedef SDL_Color Color;

Color color (unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0xFF);

namespace graphics {

	int InitGraphics(Config *conf);
	void QuitGraphics();
	Color getBackgroundColor();
	void setBackgroundColor(Color color);
	void setLineThickness(float thickness);
	float getLineThickness();
	Color getColor();
	void setColor(Color color);
	void rectangle(float x, float y, float width, float height, bool filled = false);
	void rectangle(float x, float y, float width, float height, Color col, bool filled = false);
	void rectangleFilled(float x, float y, float width, float height);
	void rectangleFilled(float x, float y, float width, float height, Color col);
	void circle(float x, float y, float radius, bool filled = false);
	void circle(float x, float y, float radius, Color col, bool filled = false);
	void circleFilled(float x, float y, float radius);
	void circleFilled(float x, float y, float radius, Color col);
	void triangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled = false);
	void triangle(float x1, float y1, float x2, float y2, float x3, float y3, Color col, bool filled = false);
	void triangleFilled(float x1, float y1, float x2, float y2, float x3, float y3);
	void triangleFilled(float x1, float y1, float x2, float y2, float x3, float y3, Color col);
	void ellipse(float x, float y, float rx, float ry, bool filled = false);
	void ellipse(float x, float y, float rx, float ry, float degrees, bool filled = false);
	void ellipse(float x, float y, float rx, float ry, Color col, bool filled = false);
	void ellipse(float x, float y, float rx, float ry, float degrees, Color col, bool filled = false);
	void ellipseFilled(float x, float y, float rx, float ry);
	void ellipseFilled(float x, float y, float rx, float ry, float degrees);
	void ellipseFilled(float x, float y, float rx, float ry, Color col);
	void ellipseFilled(float x, float y, float rx, float ry, float degrees, Color col);
	void polygon(std::vector<float> vertices, bool filled = false);
	void polygon(std::vector<float> vertices, Color col, bool filled = false);
	void polygonFilled(std::vector<float> vertices);
	void polygonFilled(std::vector<float> vertices, Color col);
	void point(float x, float y);
	void point(float x, float y, Color col);
	void line(float x1, float y1, float x2, float y2);
	void line(float x1, float y1, float x2, float y2, Color col);
	void line(std::vector<float> lines);
	void line(std::vector<float> lines, Color col);
	void arc(float x, float y, float r, float a1, float a2, bool filled = false);
	void arc(float x, float y, float r, float a1, float a2, Color col, bool filled = false);
	void arcFilled(float x, float y, float r, float a1, float a2);
	void arcFilled(float x, float y, float r, float a1, float a2, Color col);
	void clear();
	void clear(Color col);
	void print(const char *str, float x = 0, float y = 0);
	void print(const char *str, float x, float y, Font *font);
	void print(const char *str, float x, float y, Color col);
	void print(const char *str, float x, float y, Font *font, Color col);
	void printf(float x, float y, const char *str, ...);
	void printf(float x, float y, Font *font, const char *str, ...);
	void printf(float x, float y, Color col, const char *str, ...);
	void printf(float x, float y, Font *font, Color col, const char *str, ...);
	Font *getFont();
	void setFont(Font *newFont);
	void render(Image *image, float x, float y);
	void getVirtualCoords(int x, int y, int *virtX, int *virtY);

};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_GRAPHICS
