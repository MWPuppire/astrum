#ifdef __GNUC__
#  define UNUSED(x) UNUSED_##x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_##x
#endif

extern "C" {
	#define SDL_MAIN_HANDLED
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_ttf.h>
}

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#endif

#include <functional>
#include <map>
#include <vector>
#include <filesystem>

#include "astrum/astrum.hpp"
#include "astrum/constants.hpp"
#include "astrum/window.hpp"
#include "astrum/font.hpp"
#include "astrum/keyboard.hpp"
#include "astrum/mouse.hpp"
#include "astrum/math.hpp"
#include "astrum/graphics.hpp"
#include "astrum/event.hpp"
#include "astrum/log.hpp"
#include "internals.hpp"

namespace Astrum
{

const char *VERSION = "0.1.0";
const int VERSION_MAJOR = 0;
const int VERSION_MINOR = 1;
const int VERSION_PATCH = 0;
const char *DEFAULT_TITLE = "Untitled";
const int DEFAULT_WIDTH = 640;
const int DEFAULT_HEIGHT = 480;

namespace
{
	int hasInit = 0;

	const int FRAME_VALUES = 16;

	Uint32 frametimes[FRAME_VALUES] = { 0 };
	Uint32 frametimelast = 0;
	Uint32 framecount = 0;
	double framespersecond = 0;
	Uint64 currenttime = 0;
	Uint64 lasttime = 0;

	bool isrunning = false;
	bool doquit = false;

	std::vector<std::function<void()> > cb_startup;
	std::vector<std::function<void()> > cb_draw;
	std::vector<std::function<bool()> > cb_quit;
	std::vector<std::function<void(Sint32, Sint32)> > cb_resize;
	std::vector<std::function<void(bool)> > cb_visible;
	std::vector<std::function<void(bool)> > cb_focus;
	std::vector<std::function<void(Sint32, Sint32)> > cb_moved;
	std::vector<std::function<void(Key, Uint16, bool)> > cb_keypressed;
	std::vector<std::function<void(Key)> > cb_keyreleased;
	std::vector<std::function<void(char *)> > cb_textinput;
	std::vector<std::function<void(char *, Sint32, Sint32)> > cb_textedited;
	std::vector<std::function<void(Sint32, Sint32, Sint32, Sint32)> > cb_mousemoved;
	std::vector<std::function<void(Sint32, Sint32, Sint32, Sint32)> > cb_mousepressed;
	std::vector<std::function<void(Sint32, Sint32, Sint32, Sint32)> > cb_mousereleased;
	std::vector<std::function<void(Sint32, Sint32)> > cb_wheelmoved;
	std::vector<std::function<void(bool)> > cb_mousefocus;
	std::vector<std::function<void(std::filesystem::path)> > cb_filedropped;
	std::vector<std::function<void(std::filesystem::path)> > cb_directorydropped;
#ifdef __EMSCRIPTEN__
	std::function<void(double, double)> cb_update;
#endif

	double fps;
	double dt;
};

bool handle_event(SDL_Event e)
{
	bool term = false;
	int virtX, virtY;
	switch (e.type) {
	case SDL_QUIT:
		term = true;
		for (size_t i = 0; i < cb_quit.size(); i++)
			term = cb_quit[i]() && term;
		break;
	case SDL_KEYDOWN:
		if (e.key.repeat && !keyboard::hasKeyRepeat())
			break;
		for (size_t i = 0; i < cb_keypressed.size(); i++)
			cb_keypressed[i](fromKeycode(e.key.keysym.sym),
				e.key.keysym.mod, (bool) e.key.repeat);
		break;
	case SDL_KEYUP:
		for (size_t i = 0; i < cb_keyreleased.size(); i++)
			cb_keyreleased[i](fromKeycode(e.key.keysym.sym));
		break;
	case SDL_TEXTEDITING:
		for (size_t i = 0; i < cb_textedited.size(); i++)
			cb_textedited[i](e.edit.text, e.edit.start, e.edit.length);
		break;
	case SDL_TEXTINPUT:
		for (size_t i = 0; i < cb_textinput.size(); i++)
			cb_textinput[i](e.edit.text);
		break;
	case SDL_MOUSEMOTION:
		graphics::getVirtualCoords(e.motion.x, e.motion.y, virtX, virtY);
		for (size_t i = 0; i < cb_mousemoved.size(); i++)
			cb_mousemoved[i](virtX, virtY, e.motion.xrel, e.motion.yrel);
		break;
	case SDL_MOUSEBUTTONDOWN:
		graphics::getVirtualCoords(e.motion.x, e.motion.y, virtX, virtY);
		for (size_t i = 0; i < cb_mousepressed.size(); i++)
			cb_mousepressed[i](e.button.button, virtX, virtY, e.button.clicks);
		break;
	case SDL_MOUSEBUTTONUP:
		graphics::getVirtualCoords(e.motion.x, e.motion.y, virtX, virtY);
		for (size_t i = 0; i < cb_mousereleased.size(); i++)
			cb_mousereleased[i](e.button.button, virtX, virtY, e.button.clicks);
		break;
	case SDL_MOUSEWHEEL: {
		Sint32 mul = e.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? -1 : 1;
		for (size_t i = 0; i < cb_wheelmoved.size(); i++)
			cb_wheelmoved[i](e.wheel.x * mul, e.wheel.y * mul);
		break;
	}
	case SDL_WINDOWEVENT:
		switch (e.window.event) {
		case SDL_WINDOWEVENT_SHOWN:
			for (size_t i = 0; i < cb_visible.size(); i++)
				cb_visible[i](true);
			break;
		case SDL_WINDOWEVENT_HIDDEN:
			for (size_t i = 0; i < cb_visible.size(); i++)
				cb_visible[i](false);
			break;
		case SDL_WINDOWEVENT_MOVED:
			for (size_t i = 0; i < cb_moved.size(); i++)
				cb_moved[i](e.window.data1, e.window.data2);
			break;
		case SDL_WINDOWEVENT_RESIZED:
			for (size_t i = 0; i < cb_resize.size(); i++)
				cb_resize[i](e.window.data1, e.window.data2);
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			for (size_t i = 0; i < cb_focus.size(); i++)
				cb_focus[i](true);
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			for (size_t i = 0; i < cb_focus.size(); i++)
				cb_focus[i](false);
			break;
		case SDL_WINDOWEVENT_ENTER:
			for (size_t i = 0; i < cb_mousefocus.size(); i++)
				cb_mousefocus[i](true);
			break;
		case SDL_WINDOWEVENT_LEAVE:
			for (size_t i = 0; i < cb_mousefocus.size(); i++)
				cb_mousefocus[i](false);
			break;
		}
		break;
	case SDL_DROPFILE:
		std::filesystem::path p(e.drop.file);
		bool isdir = std::filesystem::is_directory(p);
		if (isdir) {
			for (size_t i = 0; i < cb_directorydropped.size(); i++)
				cb_directorydropped[i](p);
		} else {
			for (size_t i = 0; i < cb_filedropped.size(); i++)
				cb_filedropped[i](p);
		}
		SDL_free(e.drop.file);
		break;
	}
	return term;
}

int init(Config &conf)
{
	if (hasInit)
		return 0;

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

	init = keyboard::InitKeyboard();
	if (init != 0) {
		log::error("Unable to initialize keyboard submodule\n");
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

	hasInit = 1;
	return 0;
}

void exit()
{
	SDL_Quit();
	TTF_Quit();
	IMG_Quit();

	window::QuitWindow();
	mouse::QuitMouse();
	graphics::QuitGraphics();

	hasInit = 0;
}

void run(std::function<void(double, double)> update)
{
	if (isrunning)
		return;

	isrunning = true;
	doquit = false;

	for (size_t i = 0; i < cb_startup.size(); i++)
		cb_startup[i]();

	currenttime = SDL_GetPerformanceCounter();

#ifdef __EMSCRIPTEN__
	cb_update = update;

	void (*main_loop)() = []() {
		SDL_Event e;
		Uint32 frametimesindex = framecount % FRAME_VALUES;
		Uint32 getticks = SDL_GetTicks();
		frametimes[frametimesindex] = getticks - frametimelast;
		frametimelast = getticks;
		framecount++;
		Uint32 count = framecount < FRAME_VALUES ? framecount : FRAME_VALUES;
		framespersecond = 0;
		for (Uint32 i = 0; i < count; i++)
			framespersecond += frametimes[i];
		framespersecond /= count;
		framespersecond = 1000.0 / framespersecond;
		fps = framespersecond;

		lasttime = currenttime;
		currenttime = SDL_GetPerformanceCounter();
		double deltatime = (double) ((currenttime - lasttime) / (double) SDL_GetPerformanceFrequency());
		dt = deltatime;

		cb_update(deltatime, framespersecond);

		while (SDL_PollEvent(&e)) {
			doquit = handle_event(e);
			if (doquit) {
				isrunning = false;
				emscripten_cancel_main_loop();
			}
		}

		for (size_t i = 0; i < cb_draw.size(); i++)
			cb_draw[i]();
	};
	emscripten_set_main_loop(main_loop, 0, 1);
#else
	SDL_Event e;
	while (!doquit) {
		Uint32 frametimesindex = framecount % FRAME_VALUES;
		Uint32 getticks = SDL_GetTicks();
		frametimes[frametimesindex] = getticks - frametimelast;
		frametimelast = getticks;
		framecount++;
		Uint32 count = framecount < FRAME_VALUES ? framecount : FRAME_VALUES;
		framespersecond = 0;
		for (Uint32 i = 0; i < count; i++)
			framespersecond += frametimes[i];
		framespersecond /= count;
		framespersecond = 1000.0 / framespersecond;
		fps = framespersecond;

		lasttime = currenttime;
		currenttime = SDL_GetPerformanceCounter();
		double deltatime = (double) ((currenttime - lasttime) / (double) SDL_GetPerformanceFrequency());
		dt = deltatime;

		update(deltatime, framespersecond);

		while (SDL_PollEvent(&e)) {
			doquit = handle_event(e);
			if (doquit)
				break;
		}

		for (size_t i = 0; i < cb_draw.size(); i++)
			cb_draw[i]();

		SDL_Delay(1);
	}
	isrunning = false;
#endif
}
void run(std::function<void(double)> update)
{
	auto lambda = [update](double dt, double UNUSED(fps)) { update(dt); };
	run(lambda);
}
void run(std::function<void()> update)
{
	auto lambda = [update](double UNUSED(dt), double UNUSED(fps)) { update(); };
	run(lambda);
}

double getDeltaTime()
{
	return dt;
}
double getFramesPerSecond()
{
	return fps;
}

void quit(bool checkonquit)
{
	if (!isrunning)
		return;
	if (checkonquit) {
		doquit = true;
		for (size_t i = 0; i < cb_quit.size(); i++)
			doquit = cb_quit[i]() && doquit;
	} else {
		doquit = true;
	}
}

void onquit(std::function<bool()> cb)
{
	cb_quit.push_back(cb);
}
void onquit(std::function<void()> cb)
{
	auto lambda = [cb]() { cb(); return true; };
	cb_quit.push_back(lambda);
}

void ondraw(std::function<void()> cb)
{
	cb_draw.push_back(cb);
}

void onkeypressed(std::function<void(Key, Uint16, bool)> cb)
{
	cb_keypressed.push_back(cb);
}
void onkeypressed(std::function<void(Key, Uint16)> cb)
{
	auto lambda = [cb](Key k, Uint16 m, bool UNUSED(r)) { cb(k, m); };
	cb_keypressed.push_back(lambda);
}
void onkeypressed(std::function<void(Key)> cb)
{
	auto lambda = [cb](Key k, Uint16 UNUSED(m), bool UNUSED(r)) { cb(k); };
	cb_keypressed.push_back(lambda);
}

void onkeyreleased(std::function<void(Key)> cb)
{
	cb_keyreleased.push_back(cb);
}

void onresize(std::function<void(Sint32, Sint32)> cb)
{
	cb_resize.push_back(cb);
}

void onvisible(std::function<void(bool)> cb)
{
	cb_visible.push_back(cb);
}

void onfocus(std::function<void(bool)> cb)
{
	cb_focus.push_back(cb);
}

void onmoved(std::function<void(Sint32, Sint32)> cb)
{
	cb_moved.push_back(cb);
}

void ontextinput(std::function<void(char *)> cb)
{
	cb_textinput.push_back(cb);
}

void ontextedited(std::function<void(char *, Sint32, Sint32)> cb)
{
	cb_textedited.push_back(cb);
}
void ontextedited(std::function<void(char *, Sint32)> cb)
{
	auto lambda = [cb](char *text, Sint32 start, Sint32 UNUSED(length)) { cb(text, start); };
	cb_textedited.push_back(lambda);
}
void ontextedited(std::function<void(char *)> cb)
{
	auto lambda = [cb](char *text, Sint32 UNUSED(start), Sint32 UNUSED(length)) { cb(text); };
	cb_textedited.push_back(lambda);
}

void onmousemoved(std::function<void(Sint32, Sint32, Sint32, Sint32)> cb)
{
	cb_mousemoved.push_back(cb);
}
void onmousemoved(std::function<void(Sint32, Sint32)> cb)
{
	auto lambda = [cb](Sint32 x, Sint32 y, Sint32 UNUSED(dx), Sint32 UNUSED(dy)) { cb(x, y); };
	cb_mousemoved.push_back(lambda);
}

void onmousepressed(std::function<void(Sint32, Sint32, Sint32, Sint32)> cb)
{
	cb_mousepressed.push_back(cb);
}
void onmousepressed(std::function<void(Sint32, Sint32, Sint32)> cb)
{
	auto lambda = [cb](Sint32 button, Sint32 x, Sint32 y, Sint32 UNUSED(clicks)) { cb(button, x, y); };
	cb_mousepressed.push_back(lambda);
}
void onmousepressed(std::function<void(Sint32)> cb)
{
	auto lambda = [cb](Sint32 button, Sint32 UNUSED(x), Sint32 UNUSED(y), Sint32 UNUSED(clicks)) { cb(button); };
	cb_mousepressed.push_back(lambda);
}

void onmousereleased(std::function<void(Sint32, Sint32, Sint32, Sint32)> cb)
{
	cb_mousereleased.push_back(cb);
}
void onmousereleased(std::function<void(Sint32, Sint32, Sint32)> cb)
{
	auto lambda = [cb](Sint32 button, Sint32 x, Sint32 y, Sint32 UNUSED(clicks)) { cb(button, x, y); };
	cb_mousereleased.push_back(lambda);
}
void onmousereleased(std::function<void(Sint32)> cb)
{
	auto lambda = [cb](Sint32 button, Sint32 UNUSED(x), Sint32 UNUSED(y), Sint32 UNUSED(clicks)) { cb(button); };
	cb_mousereleased.push_back(lambda);
}

void onwheelmoved(std::function<void(Sint32, Sint32)> cb)
{
	cb_wheelmoved.push_back(cb);
}

void onmousefocus(std::function<void(bool)> cb)
{
	cb_mousefocus.push_back(cb);
}

void onfiledropped(std::function<void(std::filesystem::path)> cb)
{
	cb_filedropped.push_back(cb);
}
void onfiledropped(std::function<void(const char *)> cb)
{
	auto lambda = [cb](std::filesystem::path p) { cb(p.string().c_str()); };
	cb_filedropped.push_back(lambda);
}

void ondirectorydropped(std::function<void(std::filesystem::path)> cb)
{
	cb_directorydropped.push_back(cb);
}
void ondirectorydropped(std::function<void(const char *)> cb)
{
	auto lambda = [cb](std::filesystem::path p) { cb(p.string().c_str()); };
	cb_directorydropped.push_back(lambda);
}

void onstartup(std::function<void()> cb)
{
	cb_startup.push_back(cb);
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
