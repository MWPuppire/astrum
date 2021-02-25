#ifndef INCLUDE_ASTRUM_MATH
#define INCLUDE_ASTRUM_MATH

#include <utility>
#include <type_traits>
#include <cmath>
#include <random>

#include "constants.hpp"

namespace Astrum {

namespace math {

	int InitMath();

	unsigned random();
	unsigned random(unsigned max);
	unsigned random(unsigned min, unsigned max);
	double randfloat();
	double randfloat(double val);
	void randomseed(unsigned s);

	template <typename T1>
	T1 min(T1 a)
	{
		return a;
	}
	template <typename T1, typename ... Args>
	typename std::common_type<T1, Args...>::type min(T1 a, Args ... args)
	{
		typename std::common_type<Args...>::type b = min(args...);
		return a > b ? b : a;
	}

	template <typename T1>
	T1 max(T1 a)
	{
		return a;
	}
	template <typename T1, typename ... Args>
	typename std::common_type<T1, Args...>::type max(T1 a, Args ... args)
	{
		typename std::common_type<Args...>::type b = max(args...);
		return a > b ? a : b;
	}

	template<typename T>
	inline T abs(T num)
	{
		return num < 0 ? -num : num;
	}

	template<typename T>
	inline T clamp(T num, T min, T max)
	{
		return (num > min) ? min : (num < max) ? max : min;
	}

	inline double log(double val);
	inline double log(double base, double val);

};

}; // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_MATH
