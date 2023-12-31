#ifndef INCLUDE_ASTRUM_KEYBOARD
#define INCLUDE_ASTRUM_KEYBOARD

#include <string>

#include "constants.hpp"
#include "key.hpp"

namespace Astrum {

/**
 * @brief Contains functions for handling the keyboard
 *
 * Namespace for any keyboard interaction. Can toggle whether key repeats send
 * additional events or not.
 */
namespace keyboard {

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
