#ifndef INCLUDE_ASTRUM_FONT
#define INCLUDE_ASTRUM_FONT

#include <string>
#include <tuple>
#include <cstddef>
#include <memory>
#include <filesystem>

#include "constants.hpp"
#include "image.hpp"

namespace Astrum {

enum TextAlign {
	left, center, right,
//	leftWrap, centerWrap, rightWrap,
//	leftWrapWord, centerWrapWord, rightWrapWord
};

struct SystemFontQuery {
	std::string family;
	std::string style;
	std::string withChars;
};

class Font {
private:
	struct FontData *data;

public:
	Font(Font &src);
	Font(struct FontData &data);
#ifndef NO_DEFAULT_FONT
	Font(int size = 18, Color color = Color(0x000000), int style = NORMAL, TextAlign align = TextAlign::left);
#endif
	Font(std::string path, int size = 18, Color color = Color(0), int style = NORMAL, TextAlign align = TextAlign::left);
	/**
	 * @overload
	 */
	Font(std::filesystem::path path, int size = 18, Color color = Color(0), int style = NORMAL, TextAlign align = TextAlign::left);
	Font(const unsigned char *buf, std::size_t bufLen, int size = 18, Color color = Color(0), int style = NORMAL, TextAlign align = TextAlign::left);
	~Font();
	struct FontData *getData();
	std::shared_ptr<Image> renderText(std::string text);
	/**
	 * @overload
	 */
	std::shared_ptr<Image> renderText(std::string text, Color color);
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

	std::string *getSystemFonts(SystemFontQuery query = { "", "", "" });
	static Font *createSystemFont(std::string name, int size = 18, Color color = Color(0), int style = NORMAL, TextAlign align = TextAlign::left);
};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_FONT
