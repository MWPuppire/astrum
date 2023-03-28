#ifndef INCLUDE_ASTRUM_SYSTEM
#define INCLUDE_ASTRUM_SYSTEM

#include "constants.hpp"

#include <string>

namespace Astrum {

namespace system {

	// vibrates the system for `seconds` seconds, if it supports that
	// TODO
	// void vibrate(double seconds);

	// returns the current user's username
	// TODO
	// std::string getUsername();

	// returns the name of the running operating system
	std::string getPlatformName();

	// opens the URL or file path with the user's default program
	bool openURL(std::string url);

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
	void setClipboardText(std::string text);

	// returns the configured system language code
	// TODO
	// std::string getLanguage();

}

}; // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_SYSTEM
