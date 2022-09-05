#include <unordered_map>
#include <tuple>
#include <optional>
#include <memory>

#include "sdl.hpp"
#include "internals.hpp"
#include "astrum/constants.hpp"
#include "astrum/mouse.hpp"
#include "astrum/astrum.hpp"
#include "astrum/graphics.hpp"
#include "astrum/image.hpp"

namespace Astrum {

Cursor::Cursor(std::shared_ptr<CursorData> data) {
	this->data = data;
}
Cursor::Cursor(Image image, int hotX, int hotY) {
	std::shared_ptr<ImageData> data = image.getData();
	SDL_Surface *surf = data->image;
	SDL_Cursor *internalCursor = SDL_CreateColorCursor(surf, hotX, hotY);
	this->data = std::make_shared<CursorData>(internalCursor);
}
std::shared_ptr<CursorData> Cursor::getData() {
	return this->data;
}

namespace mouse {

	namespace {
		std::unordered_map<MouseButton, bool> mousedown;
	};

	void addMousedown(MouseButton btn) {
		mousedown[btn] = true;
	}
	void removeMousedown(MouseButton btn) {
		mousedown[btn] = false;
	}

	std::optional<Cursor> createSystemCursor(SDL_SystemCursor id) {
		SDL_Cursor *cursor = SDL_CreateSystemCursor(id);
		if (cursor == nullptr) {
			return std::nullopt;
		}
		auto data = std::make_shared<CursorData>(cursor);
		return std::optional(Cursor(data));
	}

	std::optional<Cursor> CURSOR_ARROW = std::nullopt;
	std::optional<Cursor> CURSOR_IBEAM = std::nullopt;
	std::optional<Cursor> CURSOR_WAIT = std::nullopt;
	std::optional<Cursor> CURSOR_CROSSHAIR = std::nullopt;
	std::optional<Cursor> CURSOR_WAITARROW = std::nullopt;
	std::optional<Cursor> CURSOR_SIZENWSE = std::nullopt;
	std::optional<Cursor> CURSOR_SIZENESW = std::nullopt;
	std::optional<Cursor> CURSOR_SIZEWE = std::nullopt;
	std::optional<Cursor> CURSOR_SIZENS = std::nullopt;
	std::optional<Cursor> CURSOR_SIZEALL = std::nullopt;
	std::optional<Cursor> CURSOR_NO = std::nullopt;
	std::optional<Cursor> CURSOR_HAND = std::nullopt;

	int InitMouse() {
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

	bool isdown(MouseButton button) {
		return mousedown[button];
	}

	int getX() {
		return std::get<0>(getCoordinates());
	}

	int getY() {
		return std::get<1>(getCoordinates());
	}

	std::tuple<int, int> getCoordinates() {
		int x, y;
		SDL_GetMouseState(&x, &y);
		int virtX, virtY;
		graphics::getVirtualCoords(x, y, virtX, virtY);
		return std::make_tuple(virtX, virtY);
	}

	void setX(int x) {
		SDL_WarpMouseInWindow(nullptr, x, getY());
	}

	void setY(int y) {
		SDL_WarpMouseInWindow(nullptr, getX(), y);
	}

	void setPosition(int x, int y) {
		SDL_WarpMouseInWindow(nullptr, x, y);
	}

	bool isVisible() {
		int out = SDL_ShowCursor(SDL_QUERY);
		return out == SDL_ENABLE;
	}

	void setVisible(bool state) {
		SDL_ShowCursor(state ? SDL_ENABLE : SDL_DISABLE);
	}

	Cursor getCursor() {
		auto data = std::make_shared<CursorData>(SDL_GetCursor(), true);
		return Cursor(data);
	}

	void setCursor(Cursor cursor) {
		std::shared_ptr<CursorData> data = cursor.getData();
		SDL_SetCursor(data->cursor);
	}
};

}; // namespace Astrum
