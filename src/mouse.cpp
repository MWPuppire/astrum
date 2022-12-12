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

const std::shared_ptr<CursorData> Cursor::getData() const {
	return this->data;
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

	void InitMouse() {
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
	}

	bool isdown(MouseButton button) {
		return mousedown[button];
	}

	int getX() {
		return std::get<0>(getPosition());
	}

	int getY() {
		return std::get<1>(getPosition());
	}

	std::tuple<int, int> getPosition() {
		int x, y;
		SDL_GetMouseState(&x, &y);
		auto [virtX, virtY] = graphics::getVirtualCoords(x, y);
		return std::make_tuple(virtX, virtY);
	}

	void setX(int x) {
		auto [virtX, virtY] = graphics::getVirtualCoords(x, getY());
		SDL_WarpMouseInWindow(nullptr, virtX, virtY);
	}

	void setY(int y) {
		auto [virtX, virtY] = graphics::getVirtualCoords(getX(), y);
		SDL_WarpMouseInWindow(nullptr, virtX, virtY);
	}

	void setPosition(int x, int y) {
		auto [virtX, virtY] = graphics::getVirtualCoords(x, y);
		SDL_WarpMouseInWindow(nullptr, virtX, virtY);
	}

	bool isVisible() {
		int out = SDL_ShowCursor(SDL_QUERY);
		return out == SDL_ENABLE;
	}

	void setVisible(bool state) {
		SDL_ShowCursor(state ? SDL_ENABLE : SDL_DISABLE);
	}

	std::optional<Cursor> getCursor() {
		SDL_Cursor *cursor = SDL_GetCursor();
		if (cursor == nullptr)
			return std::nullopt;
		auto data = std::make_shared<CursorData>(cursor, true);
		return std::optional(Cursor(data));
	}

	void setCursor(Cursor cursor) {
		std::shared_ptr<CursorData> data = cursor.getData();
		SDL_SetCursor(data->cursor);
	}
};

}; // namespace Astrum
