#include <string>
#include <functional>
#include <cstdarg>
#include <cstdio>

extern "C" {
	#define SDL_MAIN_HANDLED
	#include <SDL2/SDL.h>
};

#include "astrum/log.hpp"
#include "astrum/util.hpp"

namespace Astrum {

namespace log {
	void defaultWriteFunction(LogCategory cat, std::string str)
	{
		SDL_LogPriority priority;
		switch (cat) {
		case LogCategory::debug:
			priority = SDL_LOG_PRIORITY_DEBUG;
			break;
		case LogCategory::info:
			priority = SDL_LOG_PRIORITY_INFO;
			break;
		case LogCategory::warn:
			priority = SDL_LOG_PRIORITY_WARN;
			break;
		case LogCategory::error:
			priority = SDL_LOG_PRIORITY_ERROR;
			break;
		}
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, priority,
			"%s", str.c_str());
	}

	namespace {
#ifdef DEBUG
		LogCategory outputLevel = LogCategory::debug;
#else
		LogCategory outputLevel = LogCategory::info;
#endif
		std::function<void(LogCategory, std::string)> writeFunction
			= defaultWriteFunction;
	};

	void setLogPriority(LogCategory priority) {
		outputLevel = priority;
	}
	LogCategory logPriority() {
		return outputLevel;
	}

	void vlog(LogCategory cat, std::string format, va_list args)
	{
		// if the message is below the output level, it is ignored
		if (cat < outputLevel)
			return;
		std::string str = util::vstrformat(format, args);
		writeFunction(cat, str);
	}
	void log(LogCategory cat, std::string format, ...)
	{
		std::va_list args;
		va_start(args, format);
		vlog(cat, format, args);
		va_end(args);
	}
	void log(std::string format, ...)
	{
		std::va_list args;
		va_start(args, format);
		vlog(LogCategory::info, format, args);
		va_end(args);
	}

	void info(std::string format, ...)
	{
		std::va_list args;
		va_start(args, format);
		vlog(LogCategory::info, format, args);
		va_end(args);
	}
	void debug(std::string format, ...)
	{
		std::va_list args;
		va_start(args, format);
		vlog(LogCategory::debug, format, args);
		va_end(args);
	}
	void warn(std::string format, ...)
	{
		std::va_list args;
		va_start(args, format);
		vlog(LogCategory::warn, format, args);
		va_end(args);
	}
	void error(std::string format, ...)
	{
		std::va_list args;
		va_start(args, format);
		vlog(LogCategory::error, format, args);
		va_end(args);
	}

}

}; // namespace Astrum
