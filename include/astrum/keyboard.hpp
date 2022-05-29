#ifndef INCLUDE_ASTRUM_KEYBOARD
#define INCLUDE_ASTRUM_KEYBOARD

#include <string>

#include "constants.hpp"
#include "key.hpp"

namespace Astrum {

/**
 * @brief Class for handling the keyboard
 *
 * Class for handling the keyboard;
 * stores held keys and interfaces with
 * the system's clipboard.
 */
namespace keyboard {

	int InitKeyboard();
	bool isdown(Key key);
	bool isdown(const char *keystr);
	bool isdown(std::string keystr);
	bool hasClipboardText();
	char *getClipboardText();
	int setClipboardText(char *text);
	bool hasKeyRepeat();
	void setKeyRepeat(bool enable);

};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_KEYBOARD
