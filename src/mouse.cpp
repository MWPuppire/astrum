extern "C" {
	#define SDL_MAIN_HANDLED
	#include <SDL2/SDL.h>
}

#include <map>
#include <vector>
#include <tuple>

#include "astrum/constants.hpp"
#include "astrum/mouse.hpp"
#include "astrum/astrum.hpp"
#include "astrum/graphics.hpp"
#include "astrum/image.hpp"
#include "internals.hpp"

namespace Astrum
{

Cursor::Cursor(Cursor &cursor)
{
	CursorData otherData = *cursor.data;
	this->data = new CursorData(otherData);
}
Cursor::Cursor(std::shared_ptr<Image> image, int hotX, int hotY)
{
	ImageData *data = image->getData();
	SDL_Surface *surf = data->image;
	SDL_Cursor *internalCursor = SDL_CreateColorCursor(surf, hotX, hotY);
	this->data = new CursorData(internalCursor);
}
Cursor::Cursor(CursorData &data)
{
	this->data = new CursorData(data);
}
Cursor::~Cursor()
{
	SDL_FreeCursor(this->data->cursor);
}
CursorData *Cursor::getData()
{
	return this->data;
}

namespace mouse
{

	namespace
	{
		std::map<MouseButton, bool> mousedown;
	};

	std::shared_ptr<Cursor> createSystemCursor(SDL_SystemCursor id) {
		CursorData data = { SDL_CreateSystemCursor(id) };
		Cursor *cursor = new Cursor(data);
		return std::shared_ptr<Cursor>(cursor);
	}

	std::shared_ptr<Cursor> CURSOR_ARROW;
	std::shared_ptr<Cursor> CURSOR_IBEAM;
	std::shared_ptr<Cursor> CURSOR_WAIT;
	std::shared_ptr<Cursor> CURSOR_CROSSHAIR;
	std::shared_ptr<Cursor> CURSOR_WAITARROW;
	std::shared_ptr<Cursor> CURSOR_SIZENWSE;
	std::shared_ptr<Cursor> CURSOR_SIZENESW;
	std::shared_ptr<Cursor> CURSOR_SIZEWE;
	std::shared_ptr<Cursor> CURSOR_SIZENS;
	std::shared_ptr<Cursor> CURSOR_SIZEALL;
	std::shared_ptr<Cursor> CURSOR_NO;
	std::shared_ptr<Cursor> CURSOR_HAND;

	int InitMouse()
	{
		auto add_mousedown = [](MouseButton btn) { mousedown[btn] = true; };
		auto remove_mousedown = [](MouseButton btn) { mousedown[btn] = false; };

		onmousepressed(add_mousedown);
		onmousereleased(remove_mousedown);

		CURSOR_ARROW     = createSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
		CURSOR_IBEAM     = createSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
		CURSOR_WAIT      = createSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
		CURSOR_CROSSHAIR = createSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
		CURSOR_WAITARROW = createSystemCursor(SDL_SYSTEM_CURSOR_WAITARROW);
		CURSOR_SIZENWSE  = createSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
		CURSOR_SIZENESW  = createSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
		CURSOR_SIZEWE    = createSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
		CURSOR_SIZENS    = createSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
		CURSOR_SIZEALL   = createSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
		CURSOR_NO        = createSystemCursor(SDL_SYSTEM_CURSOR_NO);
		CURSOR_HAND      = createSystemCursor(SDL_SYSTEM_CURSOR_HAND);

		return 0;
	}

	bool isdown(MouseButton button)
	{
		return mousedown[button];
	}

	int getX()
	{
		return std::get<0>(getCoordinates());
	}

	int getY()
	{
		return std::get<1>(getCoordinates());
	}

	std::tuple<int, int> getCoordinates()
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		int virtX, virtY;
		graphics::getVirtualCoords(x, y, virtX, virtY);
		return std::make_tuple(virtX, virtY);
	}

	void setX(int x)
	{
		SDL_WarpMouseInWindow(nullptr, x, getY());
	}

	void setY(int y)
	{
		SDL_WarpMouseInWindow(nullptr, getX(), y);
	}

	void setPosition(int x, int y)
	{
		SDL_WarpMouseInWindow(nullptr, x, y);
	}

	bool isVisible()
	{
		int out = SDL_ShowCursor(SDL_QUERY);
		return out == SDL_ENABLE;
	}

	void setVisible(bool state)
	{
		SDL_ShowCursor(state ? SDL_ENABLE : SDL_DISABLE);
	}

	std::shared_ptr<Cursor> getCursor()
	{
		CursorData data = { SDL_GetCursor() };
		Cursor *cursor = new Cursor(data);
		return std::shared_ptr<Cursor>(cursor);
	}

	void setCursor(std::shared_ptr<Cursor> cursor)
	{
		CursorData *data = cursor->getData();
		SDL_SetCursor(data->cursor);
	}
};

}; // namespace Astrum
