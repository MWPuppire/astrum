#ifdef __EMSCRIPTEN__
#	include <emscripten.h>
#endif

#include <functional>
#include <optional>
#include <filesystem>
#include <string>

#include "sdl.hpp"
#include "internals.hpp"
#include "astrum/astrum.hpp"
#include "astrum/constants.hpp"
#include "astrum/window.hpp"
#include "astrum/font.hpp"
#include "astrum/keyboard.hpp"
#include "astrum/mouse.hpp"
#include "astrum/math.hpp"
#include "astrum/graphics.hpp"
#include "astrum/log.hpp"
#include "astrum/filesystem.hpp"
#include "astrum/timer.hpp"

namespace Astrum {

const std::string VERSION = "0.1.0";
const int VERSION_MAJOR = 0;
const int VERSION_MINOR = 1;
const int VERSION_PATCH = 0;
const std::string DEFAULT_TITLE = "Untitled";
const std::string DEFAULT_ORG = "Example";
const int DEFAULT_WIDTH = 640;
const int DEFAULT_HEIGHT = 480;

namespace {
	int hasInit = 0;

	bool isrunning = false;
	bool doquit = false;

	std::optional<std::function<void()> > cb_startup;
	std::optional<std::function<void()> > cb_draw;
	std::optional<std::function<bool()> > cb_quit;
	std::optional<std::function<void(int, int)> > cb_resize;
	std::optional<std::function<void(bool)> > cb_visible;
	std::optional<std::function<void(bool)> > cb_focus;
	std::optional<std::function<void(int, int)> > cb_moved;
	std::optional<std::function<void(Key, KeyMod, bool)> > cb_keypressed;
	std::optional<std::function<void(Key)> > cb_keyreleased;
	std::optional<std::function<void(std::string)> > cb_textinput;
	std::optional<std::function<void(std::string, int, int)> > cb_textedited;
	std::optional<std::function<void(int, int, int, int)> > cb_mousemoved;
	std::optional<std::function<void(MouseButton, int, int, int)> > cb_mousepressed;
	std::optional<std::function<void(MouseButton, int, int, int)> > cb_mousereleased;
	std::optional<std::function<void(int, int)> > cb_wheelmoved;
	std::optional<std::function<void(bool)> > cb_mousefocus;
	std::optional<std::function<void(std::filesystem::path)> > cb_filedropped;
	std::optional<std::function<void(std::filesystem::path)> > cb_directorydropped;
#ifdef __EMSCRIPTEN__
	std::function<void(double)> cb_update;
#endif
};

void handle_event(const SDL_Event &e) {
	int virtX, virtY;
	Key key;
	KeyMod mod;
	MouseButton btn;
	switch (e.type) {
	case SDL_QUIT:
		doquit = true;
		if (cb_quit)
			doquit = (*cb_quit)() && doquit;
		break;
	case SDL_KEYDOWN:
		if (e.key.repeat && !keyboard::hasKeyRepeat())
			break;
		key = fromKeycode(e.key.keysym.sym);
		mod = fromSDLMod(e.key.keysym.mod);
		keyboard::addKeydown(key);
		if (cb_keypressed)
			(*cb_keypressed)(key, mod, (bool) e.key.repeat);
		break;
	case SDL_KEYUP:
		key = fromKeycode(e.key.keysym.sym);
		keyboard::removeKeydown(key);
		if (cb_keyreleased)
			(*cb_keyreleased)(key);
		break;
	case SDL_TEXTEDITING:
		if (cb_textedited)
			(*cb_textedited)(e.edit.text, e.edit.start, e.edit.length);
		break;
	case SDL_TEXTINPUT:
		if (cb_textinput)
			(*cb_textinput)(e.edit.text);
		break;
	case SDL_MOUSEMOTION:
		graphics::getVirtualCoords(e.motion.x, e.motion.y, virtX, virtY);
		if (cb_mousemoved)
			(*cb_mousemoved)(virtX, virtY, e.motion.xrel, e.motion.yrel);
		break;
	case SDL_MOUSEBUTTONDOWN:
		btn = fromMouseBtn(e.button.button);
		graphics::getVirtualCoords(e.motion.x, e.motion.y, virtX, virtY);
		mouse::addMousedown(btn);
		if (cb_mousepressed)
			(*cb_mousepressed)(btn, virtX, virtY, e.button.clicks);
		break;
	case SDL_MOUSEBUTTONUP:
		btn = fromMouseBtn(e.button.button);
		graphics::getVirtualCoords(e.motion.x, e.motion.y, virtX, virtY);
		mouse::removeMousedown(btn);
		if (cb_mousereleased)
			(*cb_mousereleased)(btn, virtX, virtY, e.button.clicks);
		break;
	case SDL_MOUSEWHEEL: {
		int mul = e.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? -1 : 1;
		if (cb_wheelmoved)
			(*cb_wheelmoved)(e.wheel.x * mul, e.wheel.y * mul);
		break;
	}
	case SDL_WINDOWEVENT:
		switch (e.window.event) {
		case SDL_WINDOWEVENT_SHOWN:
			if (cb_visible)
				(*cb_visible)(true);
			break;
		case SDL_WINDOWEVENT_HIDDEN:
			if (cb_visible)
				(*cb_visible)(false);
			break;
		case SDL_WINDOWEVENT_MOVED:
			if (cb_moved)
				(*cb_moved)(e.window.data1, e.window.data2);
			break;
		case SDL_WINDOWEVENT_RESIZED:
			if (cb_resize)
				(*cb_resize)(e.window.data1, e.window.data2);
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			if (cb_focus)
				(*cb_focus)(true);
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			if (cb_focus)
				(*cb_focus)(false);
			break;
		case SDL_WINDOWEVENT_ENTER:
			if (cb_mousefocus)
				(*cb_mousefocus)(true);
			break;
		case SDL_WINDOWEVENT_LEAVE:
			if (cb_mousefocus)
				(*cb_mousefocus)(false);
			break;
		case SDL_WINDOWEVENT_MAXIMIZED:
		case SDL_WINDOWEVENT_RESTORED:
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			window::recalculateDimensions();
			break;
		}
		break;
	case SDL_DROPFILE:
		std::filesystem::path p(e.drop.file);
		bool isdir = std::filesystem::is_directory(p);
		if (isdir) {
			if (cb_directorydropped)
				(*cb_directorydropped)(p);
		} else {
			if (cb_filedropped)
				(*cb_filedropped)(p);
		}
		SDL_free(e.drop.file);
		break;
	}
}

int init(Config &conf) {
	if (hasInit)
		return 0;

	SDL_SetHint(SDL_HINT_MAC_BACKGROUND_APP,
		(conf.windowHeadless || conf.allowNoWindow) ? "1" : "0");
	SDL_SetMainReady();
	int init = SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO
		| SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);
	if (init != 0) {
		log::error("Unable to initialize SDL: %s\n", SDL_GetError());
		return init;
	}

	init = TTF_Init();
	if (init != 0) {
		log::error("Unable to initialize SDL_TTF: %s\n", TTF_GetError());
		return init;
	}

#ifdef __EMSCRIPTEN__
	int imageFlags = 0;
#else
	int imageFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
#endif
	int imageInit = IMG_Init(imageFlags);
	if ((imageInit & imageFlags) != imageFlags) {
		log::error("Failed to initialize SDL_image: %s\n", IMG_GetError());
		return 1;
	}

	init = window::InitWindow(conf);
	if (init != 0) {
		log::error("Unable to initialize window submodule\n");
		return init;
	}

	init = graphics::InitGraphics(conf);
	if (init != 0) {
		log::error("Unable to initialize graphics submodule\n");
		return init;
	}

	init = mouse::InitMouse();
	if (init != 0) {
		log::error("Unable to initialize mouse submodule\n");
		return init;
	}

	init = math::InitMath();
	if (init != 0) {
		log::error("Unable to initialize math submodule\n");
		return init;
	}

	init = filesystem::InitFS(conf);
	if (init != 0) {
		log::error("Unable to initialize filesystem submodule\n");
		return init;
	}

	init = timer::InitTimer();
	if (init != 0) {
		log::error("Unable to initialize timer submodule\n");
		return init;
	}

#ifdef __EMSCRIPTEN__
	emscripten_set_window_title(conf.appName.c_str());
#endif

	hasInit = 1;
	return 0;
}

void exit() {
	window::QuitWindow();
	graphics::QuitGraphics();
	filesystem::QuitFS();

	SDL_Quit();
	TTF_Quit();
	IMG_Quit();

	hasInit = 0;
}

#ifdef __EMSCRIPTEN__
void main_loop() {
	SDL_Event e;
	double dt = timer::step();

	while (SDL_PollEvent(&e)) {
		handle_event(e);
		if (doquit) {
			isrunning = false;
			emscripten_cancel_main_loop();
		}
	}

	cb_update(dt);

	graphics::drawframe();
	if (cb_draw)
		(*cb_draw)();
};
#endif

void run(std::function<void(double)> update) {
	if (isrunning)
		return;

	isrunning = true;
	doquit = false;

	if (cb_startup)
		(*cb_startup)();

	// don't count time from start-up function in dt
	timer::step();

#ifdef __EMSCRIPTEN__
	cb_update = update;
	emscripten_set_main_loop(main_loop, 0, 1);
#else
	SDL_Event e;
	while (!doquit) {
		double dt = timer::step();

		while (SDL_PollEvent(&e)) {
			handle_event(e);
			if (doquit)
				break;
		}

		update(dt);

		graphics::drawframe();
		if (cb_draw)
			(*cb_draw)();

		SDL_Delay(1);
	}
	isrunning = false;
#endif
}
void run(std::function<void()> update) {
	auto lambda = [update](double UNUSED(dt)) { update(); };
	run(lambda);
}

void quit(bool checkonquit) {
	if (!isrunning)
		return;
	if (checkonquit) {
		doquit = true;
		if (cb_quit)
			doquit = (*cb_quit)() && doquit;
	} else {
		doquit = true;
	}
}

void onquit(std::function<bool()> cb) {
	cb_quit = cb;
}
void onquit(std::function<void()> cb) {
	auto lambda = [cb]() { cb(); return true; };
	cb_quit = lambda;
}

void ondraw(std::function<void()> cb) {
	cb_draw = cb;
}

void onkeypressed(std::function<void(Key, KeyMod, bool)> cb) {
	cb_keypressed = cb;
}
void onkeypressed(std::function<void(Key, KeyMod)> cb) {
	auto lambda = [cb](Key k, KeyMod m, bool UNUSED(r)) { cb(k, m); };
	cb_keypressed = lambda;
}
void onkeypressed(std::function<void(Key)> cb) {
	auto lambda = [cb](Key k, KeyMod UNUSED(m), bool UNUSED(r)) { cb(k); };
	cb_keypressed = lambda;
}

void onkeyreleased(std::function<void(Key)> cb) {
	cb_keyreleased = cb;
}

void onresize(std::function<void(int, int)> cb) {
	cb_resize = cb;
}

void onvisible(std::function<void(bool)> cb) {
	cb_visible = cb;
}

void onfocus(std::function<void(bool)> cb) {
	cb_focus = cb;
}

void onmoved(std::function<void(int, int)> cb) {
	cb_moved = cb;
}

void ontextinput(std::function<void(std::string)> cb) {
	cb_textinput = cb;
}

void ontextedited(std::function<void(std::string, int, int)> cb) {
	cb_textedited = cb;
}
void ontextedited(std::function<void(std::string, int)> cb) {
	auto lambda = [cb](std::string text, int start, int UNUSED(length)) { cb(text, start); };
	cb_textedited = lambda;
}
void ontextedited(std::function<void(std::string)> cb) {
	auto lambda = [cb](std::string text, int UNUSED(start), int UNUSED(length)) { cb(text); };
	cb_textedited = lambda;
}

void onmousemoved(std::function<void(int, int, int, int)> cb) {
	cb_mousemoved = cb;
}
void onmousemoved(std::function<void(int, int)> cb) {
	auto lambda = [cb](int x, int y, int UNUSED(dx), int UNUSED(dy)) { cb(x, y); };
	cb_mousemoved = lambda;
}

void onmousepressed(std::function<void(MouseButton, int, int, int)> cb) {
	cb_mousepressed = cb;
}
void onmousepressed(std::function<void(MouseButton, int, int)> cb) {
	auto lambda = [cb](MouseButton button, int x, int y, int UNUSED(clicks)) { cb(button, x, y); };
	cb_mousepressed = lambda;
}
void onmousepressed(std::function<void(MouseButton)> cb) {
	auto lambda = [cb](MouseButton button, int UNUSED(x), int UNUSED(y), int UNUSED(clicks)) { cb(button); };
	cb_mousepressed = lambda;
}

void onmousereleased(std::function<void(MouseButton, int, int, int)> cb) {
	cb_mousereleased = cb;
}
void onmousereleased(std::function<void(MouseButton, int, int)> cb) {
	auto lambda = [cb](MouseButton button, int x, int y, int UNUSED(clicks)) { cb(button, x, y); };
	cb_mousereleased = lambda;
}
void onmousereleased(std::function<void(MouseButton)> cb) {
	auto lambda = [cb](MouseButton button, int UNUSED(x), int UNUSED(y), int UNUSED(clicks)) { cb(button); };
	cb_mousereleased = lambda;
}

void onwheelmoved(std::function<void(int, int)> cb) {
	cb_wheelmoved = cb;
}

void onmousefocus(std::function<void(bool)> cb) {
	cb_mousefocus = cb;
}

void onfiledropped(std::function<void(std::filesystem::path)> cb) {
	cb_filedropped = cb;
}
void onfiledropped(std::function<void(std::string)> cb) {
	auto lambda = [cb](std::filesystem::path p) { cb(p.string()); };
	cb_filedropped = lambda;
}

void ondirectorydropped(std::function<void(std::filesystem::path)> cb) {
	cb_directorydropped = cb;
}
void ondirectorydropped(std::function<void(std::string)> cb) {
	auto lambda = [cb](std::filesystem::path p) { cb(p.string()); };
	cb_directorydropped = lambda;
}

void onstartup(std::function<void()> cb) {
	cb_startup = cb;
}

void ongamepadaxis();
void ongamepadpressed();
void ongamepadreleased();
void onjoystickadded();
void onjoystickaxis();
void onjoystickhat();
void onjoystickpressed();
void onjoystickreleased();
void onjoystickremoved();

}; // namespace Astrum
