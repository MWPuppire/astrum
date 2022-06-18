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

	Transforms() { };
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
	struct ImageData *data;

public:
	Image(Image &src);
	Image(struct ImageData &data);
	Image(std::string filename);
	/**
	 * @overload
	 */
	Image(std::filesystem::path filename);
	Image(const unsigned char *buf, std::size_t bufLen, std::string type = "");
	Image(void *pixels, int height, int width);
	~Image();

	struct ImageData *getData();

	std::shared_ptr<Transforms> getTransforms();
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
