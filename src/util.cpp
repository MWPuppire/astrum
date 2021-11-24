#include <string>
#include <cstdarg>
#include <cstdio>
#include <stdexcept>
#include <memory>

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
		int size_i = std::vsnprintf(nullptr, 0, format, tmpArgs) + 1;
		va_end(tmpArgs);
		if (size_i <= 0)
			throw std::runtime_error("Error during formatting.");
		auto size = static_cast<size_t>(size_i);
		auto buf = std::make_unique<char[]>(size);
		std::vsnprintf(buf.get(), size, format, args);
		// exclude final NULL terminator in string
		return std::string(buf.get(), buf.get() + size - 1);
	}

	std::string vstrformat(std::string format, std::va_list args)
	{
		return vstrformat(format.c_str(), args);
	}

}

}; // namespace Astrum
