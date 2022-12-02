#ifndef INCLUDE_ASTRUM_MOUSE
#define INCLUDE_ASTRUM_MOUSE

#include <tuple>
#include <optional>
#include <memory>

#include "constants.hpp"
#include "image.hpp"

namespace Astrum {

enum class MouseButton {
	LEFT, MIDDLE, RIGHT, X1, X2
};

class Cursor {
private:
	std::shared_ptr<struct CursorData> data;

public:
	Cursor(std::shared_ptr<struct CursorData> data);
	Cursor(Image image, int hotX = 0, int hotY = 0);

	const std::shared_ptr<struct CursorData> getData() const;
	std::shared_ptr<struct CursorData> getData();
};

namespace mouse {

	bool isdown(MouseButton button);
	int getX();
	int getY();
	std::tuple<int, int> getPosition();
	void setX(int x);
	void setY(int y);
	void setPosition(int x, int y);
	bool isVisible();
	void setVisible(bool state);
	std::optional<Cursor> getCursor();
	void setCursor(Cursor cursor);

	extern std::optional<Cursor> CURSOR_ARROW;
	extern std::optional<Cursor> CURSOR_IBEAM;
	extern std::optional<Cursor> CURSOR_WAIT;
	extern std::optional<Cursor> CURSOR_CROSSHAIR;
	extern std::optional<Cursor> CURSOR_WAITARROW;
	extern std::optional<Cursor> CURSOR_SIZENWSE;
	extern std::optional<Cursor> CURSOR_SIZENESW;
	extern std::optional<Cursor> CURSOR_SIZEWE;
	extern std::optional<Cursor> CURSOR_SIZENS;
	extern std::optional<Cursor> CURSOR_SIZEALL;
	extern std::optional<Cursor> CURSOR_NO;
	extern std::optional<Cursor> CURSOR_HAND;
};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_MOUSE
