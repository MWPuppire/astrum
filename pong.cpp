#include <astrum/astrum.hpp>

#define _USE_MATH_DEFINES
#include <cmath>
#include <sstream>
#include <string>

Astrum::Color white, black;
double player, computer;
double ballX, ballY;
double ballAngle;
double ballMove;
double ballSpeed;
int playerScore, computerScore;
int lastScore;
int playerControl;

void initBall()
{
	const int width  = Astrum::window::getWidth();
	const int height = Astrum::window::getHeight();
	ballAngle = (lastScore ? 195 : 15) + Astrum::math::randfloat(60.0);
	ballX = (double) width / 2.0;
	ballY = (double) height / 2.0;
	ballMove = 0.0;
	ballSpeed = width;
}

void update(double dt)
{
	if (Astrum::keyboard::isdown("escape"))
		Astrum::quit();

	// turbo
	if (Astrum::keyboard::isdown("t"))
		dt *= 5;

	const int width  = Astrum::window::getWidth();
	const int height = Astrum::window::getHeight();
	const double circleSize  = (double) width / 24.0;
	const double halfSize    = circleSize / 2.0;
	const double shapeHeight = (double) height / 4.0;
	const double shapeWidth  = (double) width / 32.0;
	const double speed = (double) height;

	ballMove += dt;

	if (playerControl) {
		if (Astrum::keyboard::isdown("down") || Astrum::keyboard::isdown("s"))
			player += speed * dt;
		if (Astrum::keyboard::isdown("up") || Astrum::keyboard::isdown("w"))
			player -= speed * dt;
	} else {
		if (player - shapeHeight / 2.0 < ballY)
			player += speed * dt;
		else if (player - shapeHeight / 2.0 > ballY)
			player -= speed * dt;
	}
	player = (player < shapeHeight ? shapeHeight : (player > height ? height : player));
	if (computer - shapeHeight / 2.0 < ballY)
		computer += speed * dt;
	else if (computer - shapeHeight / 2.0 > ballY)
		computer -= speed * dt;
	computer = (computer < shapeHeight ? shapeHeight : (computer > height ? height : computer));

	const double radian = M_PI / 180.0;
	if (ballMove >= 0.5) {
		ballX += std::cos(ballAngle * radian) * ballSpeed * dt;
		ballY += std::sin(ballAngle * radian) * ballSpeed * dt;
	}
	double playerDiff = Astrum::math::max(ballY - player, player - ballY);
	if (ballX <= (shapeWidth * 2) && playerDiff <= shapeHeight && ballY <= player) {
		ballAngle = ballAngle + 180.0 + Astrum::math::random(30.0);
		ballSpeed *= 1.03;
	} else if (ballX < halfSize) {
		computerScore++;
		lastScore = 0;
		initBall();
	}
	double computerDiff = Astrum::math::max(ballY - computer, computer - ballY);
	if (ballX >= (width - shapeWidth * 2) && computerDiff <= shapeHeight && ballY <= computer) {
		ballAngle = ballAngle + 180.0 + Astrum::math::random(30.0);
		ballSpeed *= 1.03;
	} else if (ballX > width - halfSize) {
		playerScore++;
		lastScore = 1;
		initBall();
	}
	if (ballY < halfSize) {
		ballY = halfSize;
		ballAngle = ballAngle + 180.0 + Astrum::math::random(30.0);
	} else if (ballY > height - halfSize) {
		ballY = height - halfSize;
		ballAngle = ballAngle + 180.0 + Astrum::math::random(30.0);
	}
	while (ballAngle >= 360.0)
		ballAngle -= 360.0;

	std::ostringstream scorestream;
	scorestream << "Player: " << playerScore << " - " << "Computer: " << computerScore;
	std::string str = scorestream.str();
	const char *toprint = str.c_str();
	int textWidth, textHeight;
	Astrum::Font *font = Astrum::graphics::getFont();
	font->textSize(toprint, &textWidth, &textHeight);
	Astrum::graphics::print(toprint, (width - textWidth) / 2, 5, white);
}

void draw()
{
	const int width  = Astrum::window::getWidth();
	const int height = Astrum::window::getHeight();
	const double shapeHeight = (double) height / 4.0;
	const double shapeWidth  = (double) width / 32.0;
	const double circleSize  = (double) width / 24.0;

	Astrum::graphics::rectangle(shapeWidth, player - shapeHeight, shapeWidth, shapeHeight, white, true);
	Astrum::graphics::rectangle(width - 2 * shapeWidth, computer - shapeHeight, shapeWidth, shapeHeight, white, true);
	Astrum::graphics::circle(ballX, ballY, circleSize, white, true);
}

void keypressed(Astrum::Keycode key)
{
	if (key == SDLK_RETURN)
		playerControl = playerControl ? 0 : 1;
}

void startup()
{
	white = Astrum::color(255, 255, 255);
	black = Astrum::color(0, 0, 0);
	Astrum::graphics::setBackgroundColor(black);
	const int height = Astrum::window::getHeight();
	const double shapeHeight = (double) height / 4.0;
	const double center = (double) (height - shapeHeight) / 2.0 + shapeHeight;
	player = computer = center;
	playerScore = computerScore = 0;
	lastScore = 0;
	playerControl = 1;
	initBall();
}

int main()
{
	Astrum::Config conf;
	conf.windowFullscreen = false;

	Astrum::Init(&conf);

	Astrum::onstartup(startup);
	Astrum::ondraw(draw);
	Astrum::onkeypressed(keypressed);

	Astrum::run(update);

	Astrum::Close();

	return 0;
}
