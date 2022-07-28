#ifndef INCLUDE_ASTRUM_TIMER
#define INCLUDE_ASTRUM_TIMER

#include <functional>
#include <chrono>

#include "constants.hpp"

namespace Astrum {

namespace timer {

	int InitTimer();

	void sleep(std::chrono::milliseconds interval);

	size_t setInterval(std::chrono::milliseconds interval, std::function<void()> cb);
	/**
	 * @overload
	 */
	template<class F, class... Args>
	size_t setInterval(std::chrono::milliseconds interval, F func, Args&&... args)
	{
		std::function<void()> cb = std::bind(std::forward<F>(func),
			std::forward<Args>(args)...);
		return setInterval(interval, cb);
	}
	/**
	 * @overload
	 */
	template<class F, class... Args>
	size_t setInterval(size_t ms, F func, Args&&... args)
	{
		auto interval = std::chrono::milliseconds(ms);
		std::function<void()> cb = std::bind(std::forward<F>(func),
			std::forward<Args>(args)...);
		return setInterval(interval, cb);
	}

	size_t setTimeout(std::chrono::milliseconds delay, std::function<void()> cb);
	/**
	 * @overload
	 */
	template<class F, class... Args>
	size_t setTimeout(std::chrono::milliseconds delay, F func, Args&&... args)
	{
		std::function<void()> cb = std::bind(std::forward<F>(func),
			std::forward<Args>(args)...);
		return setTimeout(delay, cb);
	}
	/**
	 * @overload
	 */
	template<class F, class... Args>
	size_t setTimeout(size_t ms, F func, Args&&... args)
	{
		auto delay = std::chrono::milliseconds(ms);
		std::function<void()> cb = std::bind(std::forward<F>(func),
			std::forward<Args>(args)...);
		return setTimeout(delay, cb);
	}

	void clearInterval(size_t idx);

	/**
	 * @brief Returns the time, in seconds, since the last call.
	 *
	 * Returns the number of seconds since this function was last called.
	 * Since this is used to determine `dt` in `update`, this should usually
	 * not be called by the user.
	 */
	double step();
	double deltatime();

}; // namespace timer

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_IMAGE
