#ifndef INCLUDE_ASTRUM_MOUSE
#define INCLUDE_ASTRUM_MOUSE

#include <tuple>
#include <memory>

#include "constants.hpp"
#include "image.hpp"

namespace Astrum {

enum class MouseButton {
	LEFT, MIDDLE, RIGHT, X1, X2
};

class Cursor {
private:
	struct CursorData *data;

public:
	Cursor(Cursor &cursor);
	Cursor(struct CursorData &data);
	Cursor(std::shared_ptr<Image> image, int hotX = 0, int hotY = 0);
	~Cursor();
	struct CursorData *getData();
};

namespace mouse {

	int InitMouse();
	bool isdown(MouseButton button);
	int getX();
	int getY();
	std::tuple<int, int> getCoordinates();
	void setX(int x);
	void setY(int y);
	void setPosition(int x, int y);
	bool isVisible();
	void setVisible(bool state);
	std::shared_ptr<Cursor> getCursor();
	void setCursor(std::shared_ptr<Cursor> cursor);

	extern std::shared_ptr<Cursor> CURSOR_ARROW;
	extern std::shared_ptr<Cursor> CURSOR_IBEAM;
	extern std::shared_ptr<Cursor> CURSOR_WAIT;
	extern std::shared_ptr<Cursor> CURSOR_CROSSHAIR;
	extern std::shared_ptr<Cursor> CURSOR_WAITARROW;
	extern std::shared_ptr<Cursor> CURSOR_SIZENWSE;
	extern std::shared_ptr<Cursor> CURSOR_SIZENESW;
	extern std::shared_ptr<Cursor> CURSOR_SIZEWE;
	extern std::shared_ptr<Cursor> CURSOR_SIZENS;
	extern std::shared_ptr<Cursor> CURSOR_SIZEALL;
	extern std::shared_ptr<Cursor> CURSOR_NO;
	extern std::shared_ptr<Cursor> CURSOR_HAND;
};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_MOUSE
