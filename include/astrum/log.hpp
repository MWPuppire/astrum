#ifndef INCLUDE_ASTRUM_LOG
#define INCLUDE_ASTRUM_LOG

#include <string>

namespace Astrum {

namespace log {

	enum class LogCategory { debug, info, warn, error };

	void setLogPriority(LogCategory priority);
	LogCategory logPriority();
	void vlog(LogCategory cat, std::string format, va_list args);
	void vlog(std::string format, va_list args);
	void log(LogCategory cat, std::string format, ...);
	void log(std::string format, ...);
	void info(std::string format, ...);
	void debug(std::string format, ...);
	void warn(std::string format, ...);
	void error(std::string format, ...);

}

}; // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_LOG
