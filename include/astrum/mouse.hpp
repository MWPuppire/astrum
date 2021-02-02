#ifndef INCLUDE_ASTRUM_MOUSE
#define INCLUDE_ASTRUM_MOUSE

extern "C" {
	#include "SDL.h"
}

#include "constants.hpp"

namespace Astrum {

typedef SDL_Cursor Cursor;
typedef SDL_SystemCursor SystemCursor;

namespace mouse {

	int InitMouse();
	void QuitMouse();
	bool isdown(int key);
	int getMouseX();
	int getMouseY();
	void setMouseX(int x);
	void setMouseY(int y);
	void setMousePosition(int x, int y);
	bool isVisible();
	void setVisible(bool state);
	Cursor *newCursor(SDL_Surface *surface, int hotX, int hotY);
	Cursor *newCursor(SystemCursor id);
	Cursor *getCursor();
	void setCursor(Cursor *cursor);
	void deleteCursor(Cursor *cursor);

	const static SystemCursor CURSOR_ARROW = SDL_SYSTEM_CURSOR_ARROW;
	const static SystemCursor CURSOR_IBEAM = SDL_SYSTEM_CURSOR_IBEAM;
	const static SystemCursor CURSOR_WAIT = SDL_SYSTEM_CURSOR_WAIT;
	const static SystemCursor CURSOR_CROSSHAIR = SDL_SYSTEM_CURSOR_CROSSHAIR;
	const static SystemCursor CURSOR_WAITARROW = SDL_SYSTEM_CURSOR_WAITARROW;
	const static SystemCursor CURSOR_SIZENWSE = SDL_SYSTEM_CURSOR_SIZENWSE;
	const static SystemCursor CURSOR_SIZENESW = SDL_SYSTEM_CURSOR_SIZENESW;
	const static SystemCursor CURSOR_SIZWWE = SDL_SYSTEM_CURSOR_SIZEWE;
	const static SystemCursor CURSOR_SIZENS = SDL_SYSTEM_CURSOR_SIZENS;
	const static SystemCursor CURSOR_SIZEALL = SDL_SYSTEM_CURSOR_SIZEALL;
	const static SystemCursor CURSOR_NO = SDL_SYSTEM_CURSOR_NO;
	const static SystemCursor CURSOR_HAND = SDL_SYSTEM_CURSOR_HAND;
};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_MOUSE
