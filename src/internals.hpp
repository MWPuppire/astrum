#ifndef INCLUDE_ASTRUM_INTERNALS
#define INCLUDE_ASTRUM_INTERNALS

extern "C" {
	#define SDL_MAIN_HANDLED
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_ttf.h>
	#include <SDL2/SDL_image.h>
};

#include "astrum/constants.hpp"
#include "astrum/font.hpp"
#include "astrum/key.hpp"
#include "astrum/image.hpp"
#include "astrum/mouse.hpp"

namespace Astrum {

struct FontData {
	TTF_Font *font;
	Color defaultColor;
	TextAlign defaultAlign;
	FontData(TTF_Font *font, Color defaultColor, TextAlign defaultAlign)
		: font(font), defaultColor(defaultColor),
		defaultAlign(defaultAlign) { }
	FontData(FontData &src) : font(src.font),
		defaultColor(src.defaultColor), defaultAlign(src.defaultAlign)
		{ }
};

struct ImageData {
	SDL_Surface *image;
	Transforms *tran;
	ImageData(SDL_Surface *surf) : image(surf)
	{
		this->tran = new Transforms();
	}
	ImageData(ImageData &src)
		: image(src.image), tran(new Transforms(*src.tran)) { }
};

struct CursorData
{
	SDL_Cursor *cursor;
	CursorData(CursorData &data) : cursor(data.cursor) { }
	CursorData(SDL_Cursor *cursor) : cursor(cursor) { }
};

struct SoundData {
	Mix_Chunk *chunk;
	int channel;
};

namespace window {
	extern SDL_Window *window;
};

constexpr MouseButton fromMouseBtn(int button)
{
	switch (button) {
		case SDL_BUTTON_LEFT: return MouseButton::LEFT;
		case SDL_BUTTON_MIDDLE: return MouseButton::MIDDLE;
		case SDL_BUTTON_RIGHT: return MouseButton::RIGHT;
		case SDL_BUTTON_X1: return MouseButton::X1;
		case SDL_BUTTON_X2: return MouseButton::X2;
		default: return MouseButton::LEFT;
	}
}

constexpr Key fromKeycode(SDL_Keycode code)
{
	switch (code) {
		case SDLK_UNKNOWN: return Key::UNKNOWN;
		case SDLK_SPACE: return Key::SPACE;
		case SDLK_ESCAPE: return Key::ESCAPE;
		case SDLK_RETURN: return Key::ENTER;
		case SDLK_TAB: return Key::TAB;
		case SDLK_BACKSPACE: return Key::BACKSPACE;
		case SDLK_EXCLAIM: return Key::BANG;
		case SDLK_QUOTEDBL: return Key::QUOTE;
		case SDLK_HASH: return Key::HASH;
		case SDLK_DOLLAR: return Key::DOLLAR;
		case SDLK_PERCENT: return Key::PERCENT;
		case SDLK_AMPERSAND: return Key::AMPERSAND;
		case SDLK_QUOTE: return Key::APOSTROPHE;
		case SDLK_LEFTPAREN: return Key::LPAREN;
		case SDLK_RIGHTPAREN: return Key::RPAREN;
		case SDLK_ASTERISK: return Key::ASTERISK;
		case SDLK_PLUS: return Key::PLUS;
		case SDLK_COMMA: return Key::COMMA;
		case SDLK_MINUS: return Key::MINUS;
		case SDLK_PERIOD: return Key::PERIOD;
		case SDLK_SLASH: return Key::SLASH;
		case SDLK_0: return Key::ZERO;
		case SDLK_1: return Key::ONE;
		case SDLK_2: return Key::TWO;
		case SDLK_3: return Key::THREE;
		case SDLK_4: return Key::FOUR;
		case SDLK_5: return Key::FIVE;
		case SDLK_6: return Key::SIX;
		case SDLK_7: return Key::SEVEN;
		case SDLK_8: return Key::EIGHT;
		case SDLK_9: return Key::NINE;
		case SDLK_COLON: return Key::COLON;
		case SDLK_SEMICOLON: return Key::SEMICOLON;
		case SDLK_LESS: return Key::LESS;
		case SDLK_EQUALS: return Key::EQUAL;
		case SDLK_GREATER: return Key::GREATER;
		case SDLK_QUESTION: return Key::QMARK;
		case SDLK_AT: return Key::AT;
		case SDLK_a: return Key::A;
		case SDLK_b: return Key::B;
		case SDLK_c: return Key::C;
		case SDLK_d: return Key::D;
		case SDLK_e: return Key::E;
		case SDLK_f: return Key::F;
		case SDLK_g: return Key::G;
		case SDLK_h: return Key::H;
		case SDLK_i: return Key::I;
		case SDLK_j: return Key::J;
		case SDLK_k: return Key::K;
		case SDLK_l: return Key::L;
		case SDLK_m: return Key::M;
		case SDLK_n: return Key::N;
		case SDLK_o: return Key::O;
		case SDLK_p: return Key::P;
		case SDLK_q: return Key::Q;
		case SDLK_r: return Key::R;
		case SDLK_s: return Key::S;
		case SDLK_t: return Key::T;
		case SDLK_u: return Key::U;
		case SDLK_v: return Key::V;
		case SDLK_w: return Key::W;
		case SDLK_x: return Key::X;
		case SDLK_y: return Key::Y;
		case SDLK_z: return Key::Z;
		case SDLK_LEFTBRACKET: return Key::LBRACKET;
		case SDLK_BACKSLASH: return Key::BACKSLASH;
		case SDLK_RIGHTBRACKET: return Key::RBRACKET;
		case SDLK_CARET: return Key::CARET;
		case SDLK_UNDERSCORE: return Key::UNDERSCORE;
		case SDLK_BACKQUOTE: return Key::BACKTICK;
		case SDLK_INSERT: return Key::INSERT;
		case SDLK_DELETE: return Key::DELETE;
		case SDLK_RIGHT: return Key::RIGHT;
		case SDLK_LEFT: return Key::LEFT;
		case SDLK_DOWN: return Key::DOWN;
		case SDLK_UP: return Key::UP;
		case SDLK_PAGEUP: return Key::PAGE_UP;
		case SDLK_PAGEDOWN: return Key::PAGE_DOWN;
		case SDLK_HOME: return Key::HOME;
		case SDLK_END: return Key::END;
		case SDLK_CAPSLOCK: return Key::CAPS_LOCK;
		case SDLK_SCROLLLOCK: return Key::SCROLL_LOCK;
		case SDLK_NUMLOCKCLEAR: return Key::NUM_LOCK;
		case SDLK_PAUSE: return Key::PAUSE;
		case SDLK_F1: return Key::F1;
		case SDLK_F2: return Key::F2;
		case SDLK_F3: return Key::F3;
		case SDLK_F4: return Key::F4;
		case SDLK_F5: return Key::F5;
		case SDLK_F6: return Key::F6;
		case SDLK_F7: return Key::F7;
		case SDLK_F8: return Key::F8;
		case SDLK_F9: return Key::F9;
		case SDLK_F10: return Key::F10;
		case SDLK_F11: return Key::F11;
		case SDLK_F12: return Key::F12;
		case SDLK_F13: return Key::F13;
		case SDLK_F14: return Key::F14;
		case SDLK_F15: return Key::F15;
		case SDLK_F16: return Key::F16;
		case SDLK_F17: return Key::F17;
		case SDLK_F18: return Key::F18;
		case SDLK_F19: return Key::F19;
		case SDLK_F20: return Key::F20;
		case SDLK_F21: return Key::F21;
		case SDLK_F22: return Key::F22;
		case SDLK_F23: return Key::F23;
		case SDLK_F24: return Key::F24;
		case SDLK_KP_0: return Key::KP_0;
		case SDLK_KP_1: return Key::KP_1;
		case SDLK_KP_2: return Key::KP_2;
		case SDLK_KP_3: return Key::KP_3;
		case SDLK_KP_4: return Key::KP_4;
		case SDLK_KP_5: return Key::KP_5;
		case SDLK_KP_6: return Key::KP_6;
		case SDLK_KP_7: return Key::KP_7;
		case SDLK_KP_8: return Key::KP_8;
		case SDLK_KP_9: return Key::KP_9;
		case SDLK_KP_PERIOD: return Key::KP_PERIOD;
		case SDLK_KP_DIVIDE: return Key::KP_DIVIDE;
		case SDLK_KP_MULTIPLY: return Key::KP_TIMES;
		case SDLK_KP_MINUS: return Key::KP_MINUS;
		case SDLK_KP_PLUS: return Key::KP_PLUS;
		case SDLK_KP_EQUALS: return Key::KP_EQUAL;
		case SDLK_KP_ENTER: return Key::KP_ENTER;
		case SDLK_LSHIFT: return Key::LSHIFT;
		case SDLK_RSHIFT: return Key::RSHIFT;
		case SDLK_LCTRL: return Key::LCTRL;
		case SDLK_RCTRL: return Key::RCTRL;
		case SDLK_LALT: return Key::LALT;
		case SDLK_RALT: return Key::RALT;
		case SDLK_LGUI: return Key::LGUI;
		case SDLK_RGUI: return Key::RGUI;
		default: return Key::UNKNOWN;
	}
}

constexpr KeyMod fromSDLMod(Uint16 mod)
{
	KeyMod out = KeyMod::NONE;
	if (mod & KMOD_LSHIFT)
		out = out | KeyMod::LSHIFT;
	if (mod & KMOD_RSHIFT)
		out = out | KeyMod::RSHIFT;
	if (mod & KMOD_LCTRL)
		out = out | KeyMod::LCTRL;
	if (mod & KMOD_RCTRL)
		out = out | KeyMod::RCTRL;
	if (mod & KMOD_LALT)
		out = out | KeyMod::LALT;
	if (mod & KMOD_RALT)
		out = out | KeyMod::RALT;
	if (mod & KMOD_LGUI)
		out = out | KeyMod::LGUI;
	if (mod & KMOD_RGUI)
		out = out | KeyMod::RGUI;
	if (mod & KMOD_NUM)
		out = out | KeyMod::NUMLOCK;
	if (mod & KMOD_CAPS)
		out = out | KeyMod::CAPSLOCK;
	if (mod & KMOD_MODE)
		out = out | KeyMod::ALT_GR;
	return out;
}

}; // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_INTERNALS
