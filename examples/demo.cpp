#include <astrum/astrum.hpp>

Astrum::Color shapeColor;
double lastColor;
double shapeX;
double shapeY;
int shape;

void update(double dt) {
	lastColor += dt;
	if (Astrum::keyboard::isdown("escape"))
		Astrum::quit();
	if (Astrum::keyboard::isdown("space") && lastColor > 0.4) {
		lastColor = 0.0;
		Astrum::Color col = Astrum::Color(Astrum::math::random(255), Astrum::math::random(255), Astrum::math::random(255));
		Astrum::graphics::setBackgroundColor(col);
		int red    = col.r - Astrum::math::max((col.r - (~col.r & 0xFF)) & 0xFF, (int) col.r, 0xFF - col.r);
		int green  = col.g - Astrum::math::max((col.g - (~col.g & 0xFF)) & 0xFF, (int) col.g, 0xFF - col.g);
		int blue   = col.b - Astrum::math::max((col.b - (~col.b & 0xFF)) & 0xFF, (int) col.b, 0xFF - col.b);
		shapeColor = Astrum::Color(red & 0xFF, green & 0xFF, blue & 0xFF);
	}
	const int width = Astrum::window::getWidth();
	const int height = Astrum::window::getHeight();
	const double speed = (double) width / 4.0;
	if (Astrum::keyboard::isdown("down") || Astrum::keyboard::isdown("s"))
		shapeY += speed * dt;
	if (Astrum::keyboard::isdown("up") || Astrum::keyboard::isdown("w"))
		shapeY -= speed * dt;
	if (Astrum::keyboard::isdown("left") || Astrum::keyboard::isdown("a"))
		shapeX -= speed * dt;
	if (Astrum::keyboard::isdown("right") || Astrum::keyboard::isdown("d"))
		shapeX += speed * dt;
	shapeX = (shapeX < 0.0 ? 0.0 : (shapeX > width ? width : shapeX));
	shapeY = (shapeY < 0.0 ? 0.0 : (shapeY > height ? height : shapeY));
}

void draw() {
	float size = (float) Astrum::window::getWidth() / 8.0;
	switch (shape) {
	case 0:
		Astrum::graphics::circle(shapeX, shapeY, size, shapeColor, true);
		break;
	case 1:
		Astrum::graphics::rectangle(shapeX - size, shapeY - size, 2.0 * size, 2.0 * size, shapeColor, true);
		break;
	case 2:
		Astrum::graphics::triangle(shapeX, shapeY - size, shapeX - size, shapeY + size, shapeX + size, shapeY + size, shapeColor, true);
		break;
	case 3:
		Astrum::graphics::ellipse(shapeX, shapeY, size / 2.0, size, shapeColor, true);
		break;
	case 4:
		Astrum::graphics::arc(shapeX, shapeY, size, 45, 315, shapeColor, true);
		break;
	}
	std::string str = Astrum::util::strformat("FPS: %f", 1 / Astrum::timer::deltatime());
	Astrum::graphics::print(str, 0, 0, shapeColor);
}

void keypressed(Astrum::Key key) {
	if (key == Astrum::Key::LSHIFT)
		shape = (shape + 1) % 5;
}

void startup() {
	lastColor = 0.0;
	shape = 0;
	shapeX = Astrum::window::getWidth() / 2.0;
	shapeY = Astrum::window::getHeight() / 2.0;
	shapeColor = Astrum::Color(0xFF, 0xFF, 0xFF);
}

int main() {
	Astrum::Config conf;
	conf.windowFullscreen = false;
	conf.windowResizable = true;
	conf.appName = "Astrum Demo";

	Astrum::init(conf);
	Astrum::onstartup(startup);
	Astrum::ondraw(draw);
	Astrum::onkeypressed(keypressed);

	Astrum::run(update);

	Astrum::exit();

	return 0;
}
