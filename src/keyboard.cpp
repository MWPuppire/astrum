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

	bool hasClipboardText()
	{
		return (bool) SDL_HasClipboardText();
	}

	std::string getClipboardText()
	{
		return SDL_GetClipboardText();
	}

	int setClipboardText(std::string text)
	{
		return SDL_SetClipboardText(text.c_str());
	}

	bool hasKeyRepeat()
	{
		return keyrepeat;
	}

	void setKeyRepeat(bool enable)
	{
		keyrepeat = enable;
	}

}

}; // namespace Astrum
