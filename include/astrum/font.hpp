#ifndef INCLUDE_ASTRUM_FONT
#define INCLUDE_ASTRUM_FONT

#include <string>
#include <tuple>
#include <cstddef>
#include <memory>

#include "constants.hpp"
#include "image.hpp"

namespace Astrum {

enum TextAlign {
	left, center, right,
//	leftWrap, centerWrap, rightWrap,
//	leftWrapWord, centerWrapWord, rightWrapWord
};

class Font {
private:
	std::unique_ptr<struct FontData> data;

public:
#ifndef NO_DEFAULT_FONT
	Font(int size = 18, Color color = Color(0x000000), int style = NORMAL, TextAlign align = TextAlign::left);
#endif
	Font(std::string path, int size = 18, Color color = Color(0x000000), int style = NORMAL, TextAlign align = TextAlign::left);
	Font(const unsigned char *buf, std::size_t bufLen, int size = 18, Color color = Color(0x000000), int style = NORMAL, TextAlign align = TextAlign::left);
	~Font();
	Image *renderText(std::string text);
	Image *renderText(std::string text, Color color);
	int textSize(std::string text, int &w, int &h);
	std::tuple<int, int> textSize(std::string text);
	int textSizef(int &w, int &h, std::string text, ...);
	std::tuple<int, int> textSizef(std::string text, ...);
	Color color();
	void setColor(Color col);
	TextAlign align();
	void setAlign(TextAlign align);

	static const int NORMAL        =  0;
	static const int BOLD          =  1;
	static const int ITALIC        =  2;
	static const int UNDERLINE     =  4;
	static const int STRIKETHROUGH =  8;
	static const int OUTLINE       = 16;
};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_FONT
