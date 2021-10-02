#ifndef INCLUDE_ASTRUM_FONT
#define INCLUDE_ASTRUM_FONT

extern "C" {
	#include "SDL.h"
	#include "SDL_ttf.h"
	#include "SDL_gpu.h"
}

#include <string>
#include <tuple>

#include "constants.hpp"

namespace Astrum {

typedef SDL_Color Color;

class Font {
	TTF_Font *font;
	Color defaultColor;

public:
	Font(int size = 18, Color color = { .r = 0, .g = 0, .b = 0, .a = 255 }, int style = NORMAL);
	Font(std::string path, int size = 18, Color color = { .r = 0, .g = 0, .b = 0, .a = 255 }, int style = NORMAL);
	Font(const char *path, int size = 18, Color color = { .r = 0, .g = 0, .b = 0, .a = 255 }, int style = NORMAL);
	Font(SDL_RWops *rw, int size = 18, Color color = { .r = 0, .g = 0, .b = 0, .a = 255 }, int style = NORMAL);
	Font(const unsigned char *buf, int bufLen, int size = 18, Color color = { .r = 0, .g = 0, .b = 0, .a = 255 }, int style = NORMAL);
	~Font();
	GPU_Image *renderText(const char *text);
	GPU_Image *renderText(const char *text, Color color);
	GPU_Image *renderText(std::string text);
	GPU_Image *renderText(std::string text, Color color);
	int textSize(const char *text, int *w, int *h);
	std::tuple<int, int> textSize(const char *text);
	int textSize(std::string text, int *w, int *h);
	std::tuple<int, int> textSize(std::string text);
	int textSizef(int *w, int *h, const char *text, ...);
	std::tuple<int, int> textSizef(const char *text, ...);
	int textSizef(int *w, int *h, std::string text, ...);
	std::tuple<int, int> textSizef(std::string text, ...);
	void setColor(Color col);
	Color getColor();

	static const int NORMAL        = TTF_STYLE_NORMAL;
	static const int BOLD          = TTF_STYLE_BOLD;
	static const int ITALIC        = TTF_STYLE_ITALIC;
	static const int UNDERLINE     = TTF_STYLE_UNDERLINE;
	static const int STRIKETHROUGH = TTF_STYLE_STRIKETHROUGH;
	static const int OUTLINE       = 16;
};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_FONT
