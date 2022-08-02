#include <unordered_map>
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
		std::unordered_map<Key, bool> keysdown;
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
