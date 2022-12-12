#ifdef __EMSCRIPTEN__
#	include <emscripten.h>
#endif

#include <functional>
#include <optional>
#include <filesystem>
#include <string>
#include <vector>
#include <utility>
#include <stdexcept>

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

bool hasInit = false;
std::vector<std::pair<void *, std::function<void(void *)>>> dropQueue;

namespace {
	bool isrunning = false;

	std::optional<std::function<void()> > startupCb;
	std::optional<std::function<void()> > drawCb;
	std::optional<std::function<void()> > quitCb;
	std::optional<std::function<void(int, int)> > resizeCb;
	std::optional<std::function<void(bool)> > visibleCb;
	std::optional<std::function<void(bool)> > focusCb;
	std::optional<std::function<void(int, int)> > movedCb;
	std::optional<std::function<void(Key, KeyMod, bool)> > keypressedCb;
	std::optional<std::function<void(Key)> > keyreleasedCb;
	std::optional<std::function<void(std::string)> > textinputCb;
	std::optional<std::function<void(std::string, int, int)> > texteditedCb;
	std::optional<std::function<void(int, int, int, int)> > mousemovedCb;
	std::optional<std::function<void(MouseButton, int, int, int)> > mousepressedCb;
	std::optional<std::function<void(MouseButton, int, int, int)> > mousereleasedCb;
	std::optional<std::function<void(int, int)> > wheelmovedCb;
	std::optional<std::function<void(bool)> > mousefocusCb;
	std::optional<std::function<void(std::filesystem::path)> > filedroppedCb;
	std::optional<std::function<void(std::filesystem::path)> > directorydroppedCb;
	std::function<void(double)> updateCb;
};

bool handleEvent(const SDL_Event &e) {
	int virtX, virtY;
	Key key;
	KeyMod mod;
	MouseButton btn;
	bool doquit = false;
	switch (e.type) {
	case SDL_QUIT:
		doquit = true;
		if (quitCb)
			(*quitCb)();
		break;
	case SDL_KEYDOWN:
		if (e.key.repeat && !keyboard::hasKeyRepeat())
			break;
		key = fromKeycode(e.key.keysym.sym);
		mod = fromSDLMod(e.key.keysym.mod);
		keyboard::addKeydown(key);
		if (keypressedCb)
			(*keypressedCb)(key, mod, (bool) e.key.repeat);
		break;
	case SDL_KEYUP:
		key = fromKeycode(e.key.keysym.sym);
		keyboard::removeKeydown(key);
		if (keyreleasedCb)
			(*keyreleasedCb)(key);
		break;
	case SDL_TEXTEDITING:
		if (texteditedCb)
			(*texteditedCb)(e.edit.text, e.edit.start, e.edit.length);
		break;
	case SDL_TEXTINPUT:
		if (textinputCb)
			(*textinputCb)(e.edit.text);
		break;
	case SDL_MOUSEMOTION:
		std::tie(virtX, virtY) = graphics::getVirtualCoords(e.motion.x, e.motion.y);
		if (mousemovedCb)
			(*mousemovedCb)(virtX, virtY, e.motion.xrel, e.motion.yrel);
		break;
	case SDL_MOUSEBUTTONDOWN:
		btn = fromMouseBtn(e.button.button);
		std::tie(virtX, virtY) = graphics::getVirtualCoords(e.button.x, e.button.y);
		mouse::addMousedown(btn);
		if (mousepressedCb)
			(*mousepressedCb)(btn, virtX, virtY, e.button.clicks);
		break;
	case SDL_MOUSEBUTTONUP:
		btn = fromMouseBtn(e.button.button);
		std::tie(virtX, virtY) = graphics::getVirtualCoords(e.button.x, e.button.y);
		mouse::removeMousedown(btn);
		if (mousereleasedCb)
			(*mousereleasedCb)(btn, virtX, virtY, e.button.clicks);
		break;
	case SDL_MOUSEWHEEL: {
		int mul = e.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? -1 : 1;
		if (wheelmovedCb)
			(*wheelmovedCb)(e.wheel.x * mul, e.wheel.y * mul);
		break;
	}
	case SDL_WINDOWEVENT:
		switch (e.window.event) {
		case SDL_WINDOWEVENT_SHOWN:
			if (visibleCb)
				(*visibleCb)(true);
			break;
		case SDL_WINDOWEVENT_HIDDEN:
			if (visibleCb)
				(*visibleCb)(false);
			break;
		case SDL_WINDOWEVENT_MOVED:
			if (movedCb)
				(*movedCb)(e.window.data1, e.window.data2);
			break;
		case SDL_WINDOWEVENT_RESIZED:
			if (resizeCb)
				(*resizeCb)(e.window.data1, e.window.data2);
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			if (focusCb)
				(*focusCb)(true);
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			if (focusCb)
				(*focusCb)(false);
			break;
		case SDL_WINDOWEVENT_ENTER:
			if (mousefocusCb)
				(*mousefocusCb)(true);
			break;
		case SDL_WINDOWEVENT_LEAVE:
			if (mousefocusCb)
				(*mousefocusCb)(false);
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
			if (directorydroppedCb)
				(*directorydroppedCb)(p);
		} else {
			if (filedroppedCb)
				(*filedroppedCb)(p);
		}
		SDL_free(e.drop.file);
		break;
	}
	return doquit;
}

void init(const Config &conf) {
	if (hasInit)
		return;

	SDL_SetMainReady();
	int init = SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO
		| SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);
	if (init != 0) {
		log::error("Unable to initialize SDL: %s\n", SDL_GetError());
		throw std::runtime_error("Unable to initialize SDL");
	}

	init = TTF_Init();
	if (init != 0) {
		log::error("Unable to initialize SDL_TTF: %s\n", TTF_GetError());
		throw std::runtime_error("Unable to initialize SDL_TTF");
	}

#ifdef __EMSCRIPTEN__
	int imageFlags = 0;
#else
	int imageFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
#endif
	int imageInit = IMG_Init(imageFlags);
	if ((imageInit & imageFlags) != imageFlags) {
		log::error("Failed to initialize SDL_image: %s\n", IMG_GetError());
		throw std::runtime_error("Failed to initialize SDL_image");
	}

	window::InitWindow(conf);
	graphics::InitGraphics(conf);
	filesystem::InitFS(conf);
	mouse::InitMouse();
	math::InitMath();
	timer::InitTimer();

	for (auto [ptr, dropFunc] : dropQueue) {
		dropFunc(ptr);
	}
	dropQueue.clear();

#ifdef __EMSCRIPTEN__
	emscripten_set_window_title(conf.appName.c_str());
#endif

	hasInit = true;
}

void exit() {
	if (!hasInit)
		return;

	window::QuitWindow();
	graphics::QuitGraphics();
	filesystem::QuitFS();

	SDL_Quit();
	TTF_Quit();
	IMG_Quit();

	hasInit = false;
}

void mainLoop() {
	SDL_Event e;
	double dt = timer::step();

	while (SDL_PollEvent(&e)) {
		bool doquit = handleEvent(e);
		if (doquit) {
#ifdef __EMSCRIPTEN__
			emscripten_cancel_main_loop();
#endif
			isrunning = false;
			return;
		}
	}

	updateCb(dt);

	graphics::drawframe();
	if (drawCb)
		(*drawCb)();
};

void run(std::function<void(double)> update) {
	if (isrunning)
		return;

	isrunning = true;

	if (startupCb)
		(*startupCb)();

	// don't count time from start-up function in dt
	timer::step();

	updateCb = update;
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(mainLoop, 0, 1);
#else
	while (isrunning) {
		SDL_Delay(1);
		mainLoop();
	}
#endif
}
void run(std::function<void()> update) {
	auto lambda = [update](double UNUSED(dt)) { update(); };
	run(lambda);
}

void quit() {
	if (!isrunning)
		return;
	SDL_Event e = {
		.type = SDL_QUIT,
	};
	SDL_PushEvent(&e);
}

void onquit(std::function<void()> cb) {
	quitCb = cb;
}

void ondraw(std::function<void()> cb) {
	drawCb = cb;
}

void onkeypressed(std::function<void(Key, KeyMod, bool)> cb) {
	keypressedCb = cb;
}
void onkeypressed(std::function<void(Key, KeyMod)> cb) {
	auto lambda = [cb](Key k, KeyMod m, bool UNUSED(r)) { cb(k, m); };
	keypressedCb = lambda;
}
void onkeypressed(std::function<void(Key)> cb) {
	auto lambda = [cb](Key k, KeyMod UNUSED(m), bool UNUSED(r)) { cb(k); };
	keypressedCb = lambda;
}

void onkeyreleased(std::function<void(Key)> cb) {
	keyreleasedCb = cb;
}

void onresize(std::function<void(int, int)> cb) {
	resizeCb = cb;
}

void onvisible(std::function<void(bool)> cb) {
	visibleCb = cb;
}

void onfocus(std::function<void(bool)> cb) {
	focusCb = cb;
}

void onmoved(std::function<void(int, int)> cb) {
	movedCb = cb;
}

void ontextinput(std::function<void(std::string)> cb) {
	textinputCb = cb;
}

void ontextedited(std::function<void(std::string, int, int)> cb) {
	texteditedCb = cb;
}
void ontextedited(std::function<void(std::string, int)> cb) {
	auto lambda = [cb](std::string text, int start, int UNUSED(length)) { cb(text, start); };
	texteditedCb = lambda;
}
void ontextedited(std::function<void(std::string)> cb) {
	auto lambda = [cb](std::string text, int UNUSED(start), int UNUSED(length)) { cb(text); };
	texteditedCb = lambda;
}

void onmousemoved(std::function<void(int, int, int, int)> cb) {
	mousemovedCb = cb;
}
void onmousemoved(std::function<void(int, int)> cb) {
	auto lambda = [cb](int x, int y, int UNUSED(dx), int UNUSED(dy)) { cb(x, y); };
	mousemovedCb = lambda;
}

void onmousepressed(std::function<void(MouseButton, int, int, int)> cb) {
	mousepressedCb = cb;
}
void onmousepressed(std::function<void(MouseButton, int, int)> cb) {
	auto lambda = [cb](MouseButton button, int x, int y, int UNUSED(clicks)) { cb(button, x, y); };
	mousepressedCb = lambda;
}
void onmousepressed(std::function<void(MouseButton)> cb) {
	auto lambda = [cb](MouseButton button, int UNUSED(x), int UNUSED(y), int UNUSED(clicks)) { cb(button); };
	mousepressedCb = lambda;
}

void onmousereleased(std::function<void(MouseButton, int, int, int)> cb) {
	mousereleasedCb = cb;
}
void onmousereleased(std::function<void(MouseButton, int, int)> cb) {
	auto lambda = [cb](MouseButton button, int x, int y, int UNUSED(clicks)) { cb(button, x, y); };
	mousereleasedCb = lambda;
}
void onmousereleased(std::function<void(MouseButton)> cb) {
	auto lambda = [cb](MouseButton button, int UNUSED(x), int UNUSED(y), int UNUSED(clicks)) { cb(button); };
	mousereleasedCb = lambda;
}

void onwheelmoved(std::function<void(int, int)> cb) {
	wheelmovedCb = cb;
}

void onmousefocus(std::function<void(bool)> cb) {
	mousefocusCb = cb;
}

void onfiledropped(std::function<void(std::filesystem::path)> cb) {
	filedroppedCb = cb;
}
void onfiledropped(std::function<void(std::string)> cb) {
	auto lambda = [cb](std::filesystem::path p) { cb(p.string()); };
	filedroppedCb = lambda;
}

void ondirectorydropped(std::function<void(std::filesystem::path)> cb) {
	directorydroppedCb = cb;
}
void ondirectorydropped(std::function<void(std::string)> cb) {
	auto lambda = [cb](std::filesystem::path p) { cb(p.string()); };
	directorydroppedCb = lambda;
}

void onstartup(std::function<void()> cb) {
	startupCb = cb;
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
