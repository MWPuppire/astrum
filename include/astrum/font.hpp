#ifndef INCLUDE_ASTRUM_FONT
#define INCLUDE_ASTRUM_FONT

#include <string>
#include <tuple>
#include <cstddef>
#include <filesystem>
#include <memory>

#include "constants.hpp"
#include "image.hpp"

namespace Astrum {

enum class TextAlign {
	Left, Center, Right,
};

struct SystemFontQuery {
	std::string family;
	std::string style;
	std::string withChars;
};

class Font {
private:
	std::shared_ptr<struct FontData> data;

public:
	Font(std::shared_ptr<struct FontData> data);
#ifndef NO_DEFAULT_FONT
	Font(int size = 18, Color color = Color(0x000000), int style = NORMAL, TextAlign align = TextAlign::Left);
#endif
	Font(std::string path, int size = 18, Color color = Color(0), int style = NORMAL, TextAlign align = TextAlign::Left);
	/**
	 * @overload
	 */
	Font(std::filesystem::path path, int size = 18, Color color = Color(0), int style = NORMAL, TextAlign align = TextAlign::Left);
	Font(const unsigned char *buf, std::size_t bufLen, int size = 18, Color color = Color(0), int style = NORMAL, TextAlign align = TextAlign::Left);

	const std::shared_ptr<struct FontData> getData() const;
	/**
	 * @overload
	 */
	std::shared_ptr<struct FontData> getData();
	Image renderText(std::string text) const;
	/**
	 * @overload
	 */
	Image renderText(std::string text, Color color) const;
	std::tuple<int, int> textSize(std::string text) const;
	Color getColor() const;
	void setColor(Color col);
	TextAlign getAlign() const;
	void setAlign(TextAlign align);

	static const int NORMAL        =  0;
	static const int BOLD          =  1;
	static const int ITALIC        =  2;
	static const int UNDERLINE     =  4;
	static const int STRIKETHROUGH =  8;
	static const int OUTLINE       = 16;

	// TODO
	// std::string *getSystemFonts(SystemFontQuery query = { "", "", "" });
	// static Font createSystemFont(std::string name, int size = 18, Color color = Color(0), int style = NORMAL, TextAlign align = TextAlign::Left);
};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_FONT
