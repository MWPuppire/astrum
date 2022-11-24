#ifndef INCLUDE_ASTRUM_LOG
#define INCLUDE_ASTRUM_LOG

#include <string>

namespace Astrum {

/**
 * @brief Contains logging functions
 *
 * Namespace for logging functions. Functions log to standard output or to a
 * file, as appropriate for the system and environment.
 */
namespace log {

	/**
	 * @brief The importance of the logged item
	 *
	 * Determines whether a message will be logged or not based on the log
	 * priority.
	 */
	enum class LogCategory { debug, info, warn, error };

	/**
	 * @brief Set the current logging priority
	 *
	 * Stop appending messages to the log with priority less than `priority`. The
	 * logging priority defaults to `LogCategory::debug` on debug builds and
	 * `LogCategory::info` otherwise.
	 * 
	 * @param priority The new priority for ignoring messages.
	 */
	void setLogPriority(LogCategory priority);
	/**
	 * @brief Get the current logging priority.
	 *
	 * Returns the current set logging priority; messages logged with priority
	 * under this will be ignored. Defaults to `LogCategory::debug` on debug
	 * builds and `LogCategory::info` otherwise.
	 * 
	 * @return LogCategory The priority for ignoring messages.
	 */
	LogCategory logPriority();
	/**
	 * @brief Logs with `va_list` instead of variadic arguments
	 *
	 * Functions the same as `log`, only takes a `va_list` argument instead of a
	 * list of separate parameters.
	 *
	 * @param cat The category logged under, for determining priority.
	 * @param format The message to be formatted and logged
	 * @param args Format parameters
	 */
	void vlog(LogCategory cat, std::string format, va_list args);
	/**
	 * @brief Defaults to `info` priority
	 *
	 * Logs with the `LogCategory::info` category by default.
	 * @overload
	 */
	void vlog(std::string format, va_list args);
	/**
	 * @brief Logs the message with the given priority
	 *
	 * Appends the message to the log. Format strings follow `printf` format.
	 * Messages are only logged if they are greater than or equal to the priority.
	 *
	 * @param cat The category logged under, for determining priority
	 * @param format The message to be formatted and logged
	 * @param ... Format parameters
	 */
	void log(LogCategory cat, std::string format, ...);
	/**
	 * @brief Defaults to `info` priority
	 *
	 * Logs with the `LogCategory::info` category by default.
	 * @overload
	 */
	void log(std::string format, ...);
	/**
	 * @brief Shorthand for logging with `info`
	 *
	 * Equivalent to `log(LogCategory::info, format, ...)`.
	 */
	void info(std::string format, ...);
	/**
	 * @brief Shorthand for logging with `debug`
	 *
	 * Equivalent to `log(LogCategory::debug, format, ...)`.
	 */
	void debug(std::string format, ...);
	/**
	 * @brief Shorthand for logging with `warn`
	 *
	 * Equivalent to `log(LogCategory::warn, format, ...)`.
	 */
	void warn(std::string format, ...);
	/**
	 * @brief Shorthand for logging with `error`
	 *
	 * Equivalent to `log(LogCategory::error, format, ...)`.
	 */
	void error(std::string format, ...);

}

}; // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_LOG
