#ifndef INCLUDE_ASTRUM_TIMER
#define INCLUDE_ASTRUM_TIMER

#include <functional>
#include <chrono>

#include "constants.hpp"

namespace Astrum {

namespace timer {

	void sleep(std::chrono::milliseconds interval);

	size_t setInterval(std::chrono::milliseconds interval, std::function<void()> cb);
	template<class F, class... Args>
	size_t setInterval(std::chrono::milliseconds interval, F func, Args&&... args)
	{
		std::function<void()> cb = std::bind(std::forward<F>(func),
			std::forward<Args>(args)...);
		return setInterval(interval, cb);
	}
	template<class F, class... Args>
	size_t setInterval(size_t ms, F func, Args&&... args)
	{
		auto interval = std::chrono::milliseconds(ms);
		std::function<void()> cb = std::bind(std::forward<F>(func),
			std::forward<Args>(args)...);
		return setInterval(interval, cb);
	}

	size_t setTimeout(std::chrono::milliseconds delay, std::function<void()> cb);
	template<class F, class... Args>
	size_t setTimeout(std::chrono::milliseconds delay, F func, Args&&... args)
	{
		std::function<void()> cb = std::bind(std::forward<F>(func),
			std::forward<Args>(args)...);
		return setTimeout(delay, cb);
	}
	template<class F, class... Args>
	size_t setTimeout(size_t ms, F func, Args&&... args)
	{
		auto delay = std::chrono::milliseconds(ms);
		std::function<void()> cb = std::bind(std::forward<F>(func),
			std::forward<Args>(args)...);
		return setTimeout(delay, cb);
	}

	void clearInterval(size_t idx);

}; // namespace timer

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_IMAGE
