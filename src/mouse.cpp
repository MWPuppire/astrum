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

namespace Astrum
{

namespace mouse
{

	namespace
	{
		std::map<int, bool> mousedown;
		std::vector<Cursor *> cursors;
	};

	int InitMouse()
	{
		auto add_mousedown = [](int key) { mousedown[key] = true; };
		auto remove_mousedown = [](int key) { mousedown[key] = false; };

		onmousepressed(add_mousedown);
		onmousereleased(remove_mousedown);

		return 0;
	}

	void QuitMouse()
	{
		for (int i = cursors.size() - 1; i >= 0; i--) {
			SDL_FreeCursor(cursors[i]);
			cursors.pop_back();
		}
	}

	bool isdown(int key)
	{
		return mousedown[key];
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

	Cursor *newCursor(Image *image, int hotX, int hotY)
	{
		SDL_Surface *surf = image->getImage();
		Cursor *cursor = SDL_CreateColorCursor(surf, hotX, hotY);
		cursors.push_back(cursor);
		return cursor;
	}
	Cursor *newCursor(SystemCursor id)
	{
		Cursor *cursor = SDL_CreateSystemCursor(id);
		cursors.push_back(cursor);
		return cursor;
	}

	Cursor *getCursor()
	{
		return SDL_GetCursor();
	}

	void setCursor(Cursor *cursor)
	{
		SDL_SetCursor(cursor);
	}

	void deleteCursor(Cursor *cursor)
	{
		SDL_FreeCursor(cursor);
		for (auto c = cursors.begin(); c != cursors.end(); c++)
			if (*c == cursor)
				cursors.erase(c);
	}

};

}; // namespace Astrum
