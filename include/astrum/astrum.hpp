#ifndef INCLUDE_ASTRUM
#define INCLUDE_ASTRUM

#ifdef __EMSCRIPTEN__
#	include <emscripten.h>
#endif

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
#include "timer.hpp"
#include "key.hpp"
#include "log.hpp"
#include "filesystem.hpp"
#include "sound.hpp"
#include "audio.hpp"
#include "system.hpp"

namespace Astrum {

void init(const Config &conf);
void exit();

void run(std::function<void(double)> update);
/**
 * @overload
 */
void run(std::function<void()> update);

void quit();

void onquit(std::function<void()> cb);

void ondraw(std::function<void()> cb);

void onstartup(std::function<void()> cb);

void onkeypressed(std::function<void(Key, KeyMod, bool)> cb);
/**
 * @overload
 */
void onkeypressed(std::function<void(Key, KeyMod)> cb);
/**
 * @overload
 */
void onkeypressed(std::function<void(Key)> cb);

void onkeyreleased(std::function<void(Key)> cb);

void onresize(std::function<void(int, int)> cb);

void onvisible(std::function<void(bool)> cb);

void onfocus(std::function<void(bool)> cb);

void onmoved(std::function<void(int, int)> cb);

void ontextinput(std::function<void(std::string)> cb);

void ontextedited(std::function<void(std::string, int, int)> cb);
/**
 * @overload
 */
void ontextedited(std::function<void(std::string, int)> cb);
/**
 * @overload
 */
void ontextedited(std::function<void(std::string)> cb);

void onmousemoved(std::function<void(int, int, int, int)> cb);
/**
 * @overload
 */
void onmousemoved(std::function<void(int, int)> cb);

void onmousepressed(std::function<void(MouseButton, int, int, int)> cb);
/**
 * @overload
 */
void onmousepressed(std::function<void(MouseButton, int, int)> cb);
/**
 * @overload
 */
void onmousepressed(std::function<void(MouseButton)> cb);

void onmousereleased(std::function<void(MouseButton, int, int, int)> cb);
/**
 * @overload
 */
void onmousereleased(std::function<void(MouseButton, int, int)> cb);
/**
 * @overload
 */
void onmousereleased(std::function<void(MouseButton)> cb);

void onwheelmoved(std::function<void(int, int)> cb);

void onmousefocus(std::function<void(bool)> cb);

void onfiledropped(std::function<void(std::filesystem::path)> cb);
/**
 * @overload
 */
void onfiledropped(std::function<void(std::string)> cb);

void ondirectorydropped(std::function<void(std::filesystem::path)> cb);
/**
 * @overload
 */
void ondirectorydropped(std::function<void(std::string)> cb);

// TODO
void ongamepadaxis();
// TODO
void ongamepadpressed();
// TODO
void ongamepadreleased();
// TODO
void onjoystickadded();
// TODO
void onjoystickaxis();
// TODO
void onjoystickhat();
// TODO
void onjoystickpressed();
// TODO
void onjoystickreleased();
// TODO
void onjoystickremoved();

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM
