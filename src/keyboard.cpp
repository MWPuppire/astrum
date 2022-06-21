#include <map>
#include <string>

#include "sdl.hpp"
#include "internals.hpp"
#include "astrum/constants.hpp"
#include "astrum/keyboard.hpp"
#include "astrum/astrum.hpp"

namespace Astrum
{

namespace keyboard
{

	namespace
	{
		std::map<Key, bool> keysdown;
		bool keyrepeat = false;
	}

	void addKeydown(Key key)
	{
		keysdown[key] = true;
	}
	void removeKeydown(Key key)
	{
		keysdown[key] = false;
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
