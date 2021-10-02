#ifndef INCLUDE_ASTRUM
#define INCLUDE_ASTRUM

extern "C" {
  #include "SDL.h"
}

#include <filesystem>
#include <functional>

#include "constants.hpp"
#include "font.hpp"
#include "graphics.hpp"
#include "keyboard.hpp"
#include "math.hpp"
#include "mouse.hpp"
#include "window.hpp"
#include "util.hpp"
#include "image.hpp"

namespace Astrum {

int init(Config *conf = new Config);
void exit();

void run(std::function<void(double, double)> update);
void run(std::function<void(double)> update);
void run(std::function<void()> update);

double getDeltaTime();
double getFramesPerSecond();

void quit(bool checkonquit = false);

void onquit(std::function<bool()> cb);
void onquit(std::function<void()> cb);

void ondraw(std::function<void()> cb);

void onstartup(std::function<void()> cb);

void onkeypressed(std::function<void(SDL_Keycode, Uint16, bool)> cb);
void onkeypressed(std::function<void(SDL_Keycode, Uint16)> cb);
void onkeypressed(std::function<void(SDL_Keycode)> cb);

void onkeyreleased(std::function<void(SDL_Keycode)> cb);

void onresize(std::function<void(Sint32, Sint32)> cb);

void onvisible(std::function<void(bool)> cb);

void onfocus(std::function<void(bool)> cb);

void onmoved(std::function<void(Sint32, Sint32)> cb);

void ontextinput(std::function<void(char *)> cb);

void ontextedited(std::function<void(char *, Sint32, Sint32)> cb);
void ontextedited(std::function<void(char *, Sint32)> cb);
void ontextedited(std::function<void(char *)> cb);

void onmousemoved(std::function<void(Sint32, Sint32, Sint32, Sint32)> cb);
void onmousemoved(std::function<void(Sint32, Sint32)> cb);

void onmousepressed(std::function<void(Sint32, Sint32, Sint32, Sint32)> cb);
void onmousepressed(std::function<void(Sint32, Sint32, Sint32)> cb);
void onmousepressed(std::function<void(Sint32)> cb);

void onmousereleased(std::function<void(Sint32, Sint32, Sint32, Sint32)> cb);
void onmousereleased(std::function<void(Sint32, Sint32, Sint32)> cb);
void onmousereleased(std::function<void(Sint32)> cb);

void onwheelmoved(std::function<void(Sint32, Sint32)> cb);

void onmousefocus(std::function<void(bool)> cb);

void onfiledropped(std::function<void(std::filesystem::path)> cb);
void onfiledropped(std::function<void(const char *)> cb);

void ondirectorydropped(std::function<void(std::filesystem::path)> cb);
void ondirectorydropped(std::function<void(const char *)> cb);

void ongamepadaxis();
void ongamepadpressed();
void ongamepadreleased();
void onjoystickadded();
void onjoystickaxis();
void onjoystickhat();
void onjoystickpressed();
void onjoystickreleased();
void onjoystickremoved();

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM
