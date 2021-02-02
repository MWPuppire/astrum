extern "C" {
	#include "SDL.h"
}

#include <map>
#include <vector>

#include "astrum/constants.hpp"
#include "astrum/mouse.hpp"
#include "astrum/astrum.hpp"

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

	int getMouseX()
	{
		int x;
		int y;
		SDL_GetRelativeMouseState(&x, &y);
		return x;
	}

	int getMouseY()
	{
		int x;
		int y;
		SDL_GetRelativeMouseState(&x, &y);
		return y;
	}

	void setMouseX(int x)
	{
		SDL_WarpMouseInWindow(NULL, x, getMouseY());
	}

	void setMouseY(int y)
	{
		SDL_WarpMouseInWindow(NULL, getMouseX(), y);
	}

	void setMousePosition(int x, int y)
	{
		SDL_WarpMouseInWindow(NULL, x, y);
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

	Cursor *newCursor(SDL_Surface *surface, int hotX, int hotY)
	{
		Cursor *cursor = SDL_CreateColorCursor(surface, hotX, hotY);
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
