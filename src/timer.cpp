#include <functional>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <shared_mutex>

#include "sdl.hpp"
#include "astrum/astrum.hpp"
#include "astrum/timer.hpp"

namespace Astrum {

namespace timer {

	namespace {
		Uint64 currenttime = 0;
		Uint64 lasttime = 0;
		double performanceFrequency;
		double dt;

		std::vector<bool> terminated;
		std::shared_mutex termMutex;
	};

	int InitTimer() {
		performanceFrequency = (double) SDL_GetPerformanceFrequency();
		return 0;
	}

	void sleep(size_t ms) {
		SDL_Delay(ms);
	}
	void sleep(std::chrono::milliseconds delay) {
		SDL_Delay(delay.count());
	}

	static size_t _setInterval(std::chrono::milliseconds interval,
		std::function<void()> cb, bool repeat) {
		std::unique_lock lock(termMutex);
		size_t thisIdx = terminated.size();
		terminated.push_back(false);
		lock.unlock();

		std::thread th([=]() {
			std::shared_lock lock(termMutex, std::defer_lock);
			auto remaining = interval;

			do {
				sleep(interval);
				auto start = std::chrono::high_resolution_clock::now();

				lock.lock();
				bool term = terminated[thisIdx];
				lock.unlock();
				if (term)
					break;

				cb();
				auto end = std::chrono::high_resolution_clock::now();
				remaining = interval - std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			} while (repeat);
		});
		th.detach();

		return thisIdx;
	}

	size_t setInterval(std::chrono::milliseconds interval, std::function<void()> cb) {
		return _setInterval(interval, cb, true);
	}

	size_t setTimeout(std::chrono::milliseconds delay, std::function<void()> cb) {
		return _setInterval(delay, cb, false);
	}

	void clearInterval(size_t idx) {
		std::unique_lock lock(termMutex);
		if (idx < terminated.size())
			terminated[idx] = true;
		lock.unlock();
	}

	double step() {
		lasttime = currenttime;
		currenttime = SDL_GetPerformanceCounter();
		dt = (double) ((currenttime - lasttime) / performanceFrequency);
		return dt;
	}

	double deltatime() {
		return dt;
	}

}; // namespace timer

}; // namespace Astrum
