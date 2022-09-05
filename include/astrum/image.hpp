#ifndef INCLUDE_ASTRUM_IMAGE
#define INCLUDE_ASTRUM_IMAGE

#include <string>
#include <cstddef>
#include <memory>
#include <filesystem>
#include <tuple>

#include "constants.hpp"

namespace Astrum {

struct Transforms {
	// Scale
	double sx;
	double sy;
	// Translate
	int dx;
	int dy;
	// Rotate
	double degrees;
	// Shear/skew
	double kx;
	double ky;

	Transforms()
		: sx(1.0), sy(1.0), dx(0), dy(0), degrees(0.0), kx(0), ky(0) { }
	Transforms(Transforms &tran)
		: sx(tran.sx), sy(tran.sy), dx(tran.dx), dy(tran.dy),
		degrees(tran.degrees), kx(tran.kx), ky(tran.ky) { }
	Transforms(double sx, double sy, double dx, double dy, double degrees,
		double kx, double ky)
		: sx(sx), sy(sy), dx(dx), dy(dy), degrees(degrees), kx(kx),
		ky(ky) { }
};

class Image {
private:
	std::shared_ptr<struct ImageData> data;

public:
	Image(std::shared_ptr<struct ImageData> data);
	Image(std::string filename);
	/**
	 * @overload
	 */
	Image(std::filesystem::path filename);
	Image(const unsigned char *buf, std::size_t bufLen, std::string type = "");
	Image(void *pixels, int height, int width);

	std::shared_ptr<struct ImageData> getData();

	Transforms *getTransforms();
	int width();
	int height();
	void scale(double factor);
	void scale(double sx, double sy);
	void translate(int dx, int dy);
	void rotate(double degrees);
	void shear(double kx, double ky);
	void resetTransforms();
};

} // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_IMAGE
