#ifndef INCLUDE_ASTRUM_EVENT
#define INCLUDE_ASTRUM_EVENT

extern "C" {
	#include "SDL.h"
}

#include <functional>

#include "constants.hpp"

namespace Astrum {

typedef SDL_EventType EventType;
typedef SDL_Event Event;

namespace event {

	void clear();
	void clear(EventType type);
	void filter(std::function<int(Event *e)> cb);
	void filter(std::function<int(void *data, Event *e)> cb, void *data);
	void filter(int (*cb)(void *, Event *), void *data);
	Event *peek();
	Event *poll();
	void push(Event *e, bool filter = true);
	void push(EventType type, bool filter = true);
	void pump();
	void quit(int status = 0);
	Uint32 registerEvent(int num = 1);
	Event *wait();

}

}; // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_EVENT
