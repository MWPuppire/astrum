#include <string>
#include <cstdarg>
#include <cstdio>

#include "astrum/constants.hpp"
#include "astrum/util.hpp"

namespace Astrum {

namespace util {

	std::string strformat(const char *format, ...)
	{
		if (format == nullptr)
			return "";
		std::va_list args;
		va_start(args, format);
		std::string out = vstrformat(format, args);
		va_end(args);
		return out;
	}

	std::string strformat(std::string format, ...)
	{
		std::va_list args;
		va_start(args, format);
		std::string out = vstrformat(format, args);
		va_end(args);
		return out;
	}

	std::string vstrformat(const char *format, std::va_list args)
	{
		if (format == nullptr)
			return "";
		// extra space for NULL terminator
		std::va_list tmpArgs;
		va_copy(tmpArgs, args);
		int size = std::vsnprintf(nullptr, 0, format, tmpArgs) + 1;
		va_end(tmpArgs);
		char buf[size];
		std::vsnprintf(buf, size, format, args);
		// exclude final NULL terminator in string
		return std::string(buf, buf + size);
	}

	std::string vstrformat(std::string format, std::va_list args)
	{
		return vstrformat(format.c_str(), args);
	}

}

}; // namespace Astrum
