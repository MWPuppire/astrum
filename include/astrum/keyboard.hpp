#ifndef INCLUDE_ASTRUM_KEYBOARD
#define INCLUDE_ASTRUM_KEYBOARD

#include <string>

#include "constants.hpp"
#include "key.hpp"

namespace Astrum {

/**
 * @brief Contains functions for handling the keyboard
 *
 * Namespace for any keyboard interaction; stores held keys and interfaces with
 * the system's clipboard. Can toggle whether key repeats send additional events
 * or not.
 */
namespace keyboard {

	int InitKeyboard();

	/**
	 * @brief Tells whether or not a key is held.
	 *
	 * Returns whether or not the specified key is currently held.
	 *
	 * @param key The keycode of the key to check for.
	 * @return True if the key is held down and false otherwise.
	 */
	bool isdown(Key key);
	/**
	 * @overload
	 */
	bool isdown(std::string keystr);

	/**
	 * @brief Check whether the clipboard has text.
	 *
	 * Returns whether or not there is text in the clipboard.
	 *
	 * @return True if the clipboard contains any text and false otherwise.
	 */
	bool hasClipboardText();

	/**
	 * @brief Get the clipboard's text.
	 *
	 * Return any text currently in the system clipboard.
	 *
	 * @return The current text in the clipboard.
	 */
	std::string getClipboardText();

	/**
	 * @brief Set the clipboard's text.
	 *
	 * Replace the text in the clipboard with the new specified text.
	 *
	 * @param text Text to set the clipboard to.
	 *
	 * @return 0 on success and negative on an error.
	 */
	int setClipboardText(std::string text);

	/** @brief Test if key repeats are on.
	 *
	 * Return a boolean value based on whether key repeats are on. If they
	 * are off, repeated keys will not emit events.
	 *
	 * @return true if key repeats are on and false otherwise.
	 */
	bool hasKeyRepeat();

	/** @brief Set whether key repeats are on.
	 *
	 * Set whether or not repeated keys will emit additional events.
	 *
	 * @param enable True to enable key repeats and false to disable them.
	 */
	void setKeyRepeat(bool enable);

};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_KEYBOARD
