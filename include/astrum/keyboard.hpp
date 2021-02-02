#ifndef INCLUDE_ASTRUM_KEYBOARD
#define INCLUDE_ASTRUM_KEYBOARD

extern "C" {
	#include "SDL.h"
}

#include <map>

#include "constants.hpp"

namespace Astrum {

typedef SDL_Keycode Keycode;
typedef SDL_Scancode Scancode;

/**
 * @brief Class for handling the keyboard
 *
 * Class for handling the keyboard;
 * stores held keys and interfaces with
 * the system's clipboard.
 */
namespace keyboard {

	int InitKeyboard();
	bool isdown(Keycode key);
	bool isdown(Scancode key);
	bool isdown(const char *name);
	bool hasClipboardText();
	char *getClipboardText();
	int setClipboardText(char *text);
	bool hasKeyRepeat();
	void setKeyRepeat(bool enable);

};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_KEYBOARD
