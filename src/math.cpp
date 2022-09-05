#include <cmath>
#include <utility>
#include <type_traits>
#include <random>

#include "astrum/constants.hpp"
#include "astrum/math.hpp"

namespace Astrum {

namespace math {

	namespace {
		unsigned seed;
		std::mt19937 rng;
	};

	// uses a Mersenne Twister, so is not
	// suitable for cryptographic purposes
	int InitMath() {
		std::random_device rd;
		seed = rd();
		rng = std::mt19937(seed);

		return 0;
	}

	unsigned random() {
		return rng();
	}
	unsigned random(unsigned max) {
		std::uniform_int_distribution<> distr(0, max);
		return distr(rng);
	}
	unsigned random(unsigned min, unsigned max) {
		std::uniform_int_distribution<> distr(min, max);
		return distr(rng);
	}

	double randfloat(double max) {
		return (double) rng() / ((double) std::mt19937::max() / max);
	}
	double randfloat(double min, double max) {
		return min + randfloat(max - min);
	}

	void randomseed(unsigned s) {
		seed = s;
		rng = std::mt19937(seed);
	}

	inline double log(double val) {
		return std::log(val);
	}
	inline double log(double base, double val) {
		return std::log(val) / std::log(base);
	}

}

}; // namespace Astrum
