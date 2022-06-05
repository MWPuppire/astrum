extern "C" {
	#define SDL_MAIN_HANDLED
	#include <SDL2/SDL.h>
}

#include <map>
#include <string>

#include "astrum/constants.hpp"
#include "astrum/keyboard.hpp"
#include "astrum/astrum.hpp"
#include "internals.hpp"

namespace Astrum
{

namespace keyboard
{

	namespace
	{
		std::map<Key, bool> keysdown;
		bool keyrepeat = false;
	}

	int InitKeyboard()
	{
		auto add_keydown = [](Key key) {
			keysdown[key] = true;
		};
		auto remove_keydown = [](Key key) {
			keysdown[key] = false;
		};

		onkeypressed(add_keydown);
		onkeyreleased(remove_keydown);

		return 0;
	}

	/**
	 * @brief Tells whether or not a key is held.
	 *
	 * Returns whether or not the specified key
	 * is currently held.
	 *
	 * @param key The keycode of the key to check for.
	 */
	bool isdown(Key key)
	{
		return keysdown[key];
	}
	bool isdown(std::string keystr)
	{
		SDL_Keycode code = SDL_GetKeyFromName(keystr.c_str());
		Key key = fromKeycode(code);
		return keysdown[key];
	}

	/**
	 * @brief Check whether the clipboard has text.
	 *
	 * Returns whether or not there is text in the
	 * clipboard.
	 */
	bool hasClipboardText()
	{
		return (bool) SDL_HasClipboardText();
	}

	/**
	 * @brief Get the clipboard's text.
	 *
	 * Return any text currently in the system
	 * clipboard.
	 */
	std::string getClipboardText()
	{
		return SDL_GetClipboardText();
	}

	/**
	 * @brief Set the clipboard's text.
	 *
	 * Replace the text in the clipboard with
	 * the new specified text.
	 *
	 * @param text Text to set the clipboard to.
	 *
	 * @return int 0 on success and negative
	 *   on an error.
	 */
	int setClipboardText(std::string text)
	{
		return SDL_SetClipboardText(text.c_str());
	}

	/** @brief Test if key repeats are on.
	 *
	 * Return a boolean value based on whether
	 * key repeats are on. If they are off,
	 * repeated keys will not emit events.
	 *
	 * @return bool true if key repeats are
	 *   on and false otherwise.
	 */
	bool hasKeyRepeat()
	{
		return keyrepeat;
	}

	/** @brief Set whether key repeats are on.
	 *
	 * Set whether or not repeated keys will
	 * emit additional events.
	 *
	 * @param enable True to enable key repeats
	 *   and false to disable them.
	 */
	void setKeyRepeat(bool enable)
	{
		keyrepeat = enable;
	}

}

}; // namespace Astrum
