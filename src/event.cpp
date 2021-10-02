#ifdef __GNUC__
#  define UNUSED(x) UNUSED_##x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_##x
#endif

extern "C" {
	#include "SDL.h"
}

#include <functional>

#include "astrum/event.hpp"
#include "astrum/constants.hpp"

namespace Astrum
{

namespace event
{

	/**
         * @brief Clears all events from the event queue.
         *
         * Clears the event queue of all events,
         * regardless of the type of event.
         */
	void clear(void)
	{
		SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
	}

	/**
         * @brief Clears events of the specified type from the queue.
         *
         * Clears the event queue of all events of the specified type.
         *
         * @param type The type of event to remove.
         */
	void clear(EventType type)
	{
		SDL_FlushEvent(type);
	}

	void filter(std::function<int(Event *e)> cb)
	{
		auto lambda = [cb](void *UNUSED(data), Event *e) { return cb(e); };
		std::function<int(void *data, Event *e)> func(lambda);
		SDL_SetEventFilter(func.target<int(void *, Event *)>(), nullptr);
	}
	void filter(std::function<int(void *data, Event *e)> cb, void *data)
	{
		SDL_SetEventFilter(cb.target<int(void *, Event *)>(), data);
	}
	void filter(int (*cb)(void *, Event *), void *data)
	{
		SDL_SetEventFilter(cb, data);
	}

	Event *peek(void)
	{
		Event *e = (Event *) malloc(sizeof(Event));
		int success = SDL_PeepEvents(e, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
		if (success < 0) {
			SDL_Log("Error peeking events: %s\n", SDL_GetError());
			return nullptr;
		} else if (success == 1) {
			return e;
		} else {
			return nullptr;
		}
	}

	Event *poll(void)
	{
		Event *e = (Event *) malloc(sizeof(Event));
		int success = SDL_PollEvent(e);
		if (success)
			return e;
		else
			return nullptr;
	}

	void pump(void)
	{
		SDL_PumpEvents();
	}

	void push(Event *e, bool filter)
	{
		if (filter) {
			int success = SDL_PushEvent(e);
			if (success < 0)
				SDL_Log("Error pushing events: %s\n", SDL_GetError());
		} else {
			SDL_PumpEvents();
			SDL_PeepEvents(e, 1, SDL_ADDEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
			int success = SDL_PushEvent(e);
			if (success < 0)
				SDL_Log("Error pushing events: %s\n", SDL_GetError());
		}
	}
	void push(EventType type, bool filter)
	{
		Event e;
		e.type = type;
		push(&e, filter);
	}

	Uint32 registerEvent(int num)
	{
		if (num < 0)
			return (Uint32) - 1;
		return SDL_RegisterEvents(num);
	}

	Event *wait(void)
	{
		Event *e = (Event *) malloc(sizeof(Event));
		int success = SDL_WaitEvent(e);
		if (success) {
			return e;
		} else {
			SDL_Log("Error waiting for events: %s\n", SDL_GetError());
			return nullptr;
		}
	}

};

}; // namespace Astrum
