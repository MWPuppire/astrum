#ifndef INCLUDE_ASTRUM_GRAPHICS
#define INCLUDE_ASTRUM_GRAPHICS

#include <vector>
#include <cstdint>
#include <string>
#include <memory>

#include "constants.hpp"
#include "font.hpp"
#include "image.hpp"

namespace Astrum {

namespace graphics {

	int InitGraphics(Config &conf);
	void QuitGraphics();
	Color getBackgroundColor();
	void setBackgroundColor(Color color);
	void setLineThickness(int thickness);
	int getLineThickness();
	Color getColor();
	void setColor(Color color);
	void rectangle(int x, int y, int width, int height, bool filled = false);
	void rectangle(int x, int y, int width, int height, Color col, bool filled = false);
	void rectangleFilled(int x, int y, int width, int height);
	void rectangleFilled(int x, int y, int width, int height, Color col);
	void circle(int x, int y, int radius, bool filled = false);
	void circle(int x, int y, int radius, Color col, bool filled = false);
	void circleFilled(int x, int y, int radius);
	void circleFilled(int x, int y, int radius, Color col);
	void triangle(int x1, int y1, int x2, int y2, int x3, int y3, bool filled = false);
	void triangle(int x1, int y1, int x2, int y2, int x3, int y3, Color col, bool filled = false);
	void triangleFilled(int x1, int y1, int x2, int y2, int x3, int y3);
	void triangleFilled(int x1, int y1, int x2, int y2, int x3, int y3, Color col);
	void ellipse(int x, int y, int rx, int ry, bool filled = false);
	void ellipse(int x, int y, int rx, int ry, Color col, bool filled = false);
	void ellipseFilled(int x, int y, int rx, int ry);
	void ellipseFilled(int x, int y, int rx, int ry, Color col);
	void polygon(std::vector<int> vertices, bool filled = false);
	void polygon(std::vector<int> vertices, Color col, bool filled = false);
	void polygonFilled(std::vector<int> vertices);
	void polygonFilled(std::vector<int> vertices, Color col);
	void point(int x, int y);
	void point(int x, int y, Color col);
	void line(int x1, int y1, int x2, int y2);
	void line(int x1, int y1, int x2, int y2, Color col);
	void line(std::vector<int> lines);
	void line(std::vector<int> lines, Color col);
	void arc(int x, int y, int r, int a1, int a2, bool filled = false);
	void arc(int x, int y, int r, int a1, int a2, Color col, bool filled = false);
	void arcFilled(int x, int y, int r, int a1, int a2);
	void arcFilled(int x, int y, int r, int a1, int a2, Color col);
	void clear();
	void clear(Color col);
	void print(std::string str, int x = 0, int y = 0);
	void print(std::string str, int x, int y, std::shared_ptr<Font> font);
	void print(std::string str, int x, int y, Color col);
	void print(std::string str, int x, int y, std::shared_ptr<Font> font, Color col);
	std::shared_ptr<Font> getFont();
	void setFont(std::shared_ptr<Font> newFont);
	void render(std::shared_ptr<Image> image, int x, int y);
	void getVirtualCoords(int x, int y, int &virtX, int &virtY);
	std::shared_ptr<Image> screenshot();

};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_GRAPHICS
