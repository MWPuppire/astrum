#include <vector>
#include <memory>

#include <astrum/astrum.hpp>

class Circle {
	int xPos;
	int yPos;
	int radius;
	Astrum::Color color;

public:
	Circle()
	{
		int width = Astrum::window::getWidth(), height =
			Astrum::window::getHeight();
		this->xPos = Astrum::math::random(1, width);
		this->yPos = Astrum::math::random(1, height);
		this->radius = 5 * Astrum::math::random(15, 30);
		this->color = Astrum::color(Astrum::math::random(256),
			Astrum::math::random(256), Astrum::math::random(256));
	}

	void update(double dt)
	{
		this->radius = radius - 5 * dt;
	}

	void draw()
	{
		Astrum::graphics::circle(this->xPos, this->yPos, this->radius,
			this->color, true);
	}
};

std::vector<std::unique_ptr<Circle>> circles;

void load()
{
	int numCircles = Astrum::math::random(17, 24);
	for (int i = 0; i < numCircles; i++)
		circles.push_back(std::make_unique<Circle>());
}

void update(double dt)
{
	if (Astrum::keyboard::isdown("escape"))
		Astrum::quit();

	for (const auto &circle : circles)
		circle->update(dt);

	if (Astrum::math::randfloat() < 4 * dt)
		circles.push_back(std::make_unique<Circle>());
}

void draw()
{
	for (const auto &circle : circles)
		circle->draw();
}

int main()
{
	Astrum::Config conf;
	conf.windowTitle = "Astrum Circles";
	conf.windowFullscreen = false;
	conf.windowResizable = true;

	Astrum::init(&conf);
	Astrum::onstartup(load);
	Astrum::ondraw(draw);
	Astrum::run(update);

	Astrum::exit();
	return 0;
}
