#include <astrum/astrum.hpp>

#include <cmath>
#include <sstream>
#include <string>
#include <tuple>

const Astrum::Color white = Astrum::color(255, 255, 255);
const Astrum::Color black = Astrum::color(0, 0, 0);
const double radian = 0.01745329;
const double velocityTransfer = 0.5;
const double ballCollisionSpeed = 1.03;

double player, computer;
double ballX, ballY;
double ballAngle, ballMove, ballSpeed, ballPow;
int playerScore, computerScore;
Astrum::Font *font;

void initBall(int lastScore)
{
	const int width  = Astrum::window::getWidth();
	const int height = Astrum::window::getHeight();
	ballAngle = (lastScore ? 195 : 15) + Astrum::math::random(4) * 15.0;
	ballX = (double) width / 2.0;
	ballY = (double) height / 2.0;
	ballMove = 0.0;
	ballSpeed = width;
	ballPow = 1.0;
}

void deflectBall(double xMod, double yMod, double xAdd, double yAdd)
{
	double ballVx = std::cos(ballAngle * radian) * ballSpeed * xMod + xAdd;
	double ballVy = std::sin(ballAngle * radian) * ballSpeed * yMod + yAdd;
	ballAngle = std::atan2(ballVy, ballVx) / radian;
	while (ballAngle < 0)
		ballAngle += 360.0;
	while (ballAngle >= 360.0)
		ballAngle -= 360.0;
	ballPow *= ballCollisionSpeed;
	ballSpeed = Astrum::window::getWidth() * ballPow;
}

double updatePlayer(double dt)
{
	const int height = Astrum::window::getHeight();
	const double shapeHeight = (double) height / 4.0;
	const double speed = (double) height;

	double playerDir = 0.0;
	if (Astrum::keyboard::isdown("down") || Astrum::keyboard::isdown("s")) {
		player += speed * dt;
		playerDir += 270.0;
	}
	if (Astrum::keyboard::isdown("up") || Astrum::keyboard::isdown("w")) {
		player -= speed * dt;
		playerDir += 90.0;
	}

	if (player < shapeHeight)
		player = shapeHeight;
	else if (player > height)
		player = height;

	double playerVel = speed * std::cos(playerDir * radian);
	return playerVel;
}

double updateComputer(double dt)
{
	const int height = Astrum::window::getHeight();
	const double shapeHeight = (double) height / 4.0;
	const double speed = (double) height;

	double compDir = 0.0;
	if (computer - shapeHeight / 2.0 < ballY) {
		computer += speed * dt;
		compDir += 270.0;
	} else if (computer - shapeHeight / 2.0 > ballY) {
		computer -= speed * dt;
		compDir += 90.0;
	}

	if (computer < shapeHeight)
		computer = shapeHeight;
	else if (computer > height)
		computer = height;

	double computerVel = speed * std::cos(compDir * radian);
	return computerVel;
}

void updateBall(double dt, double playerVel, double computerVel)
{
	const int width  = Astrum::window::getWidth();
	const int height = Astrum::window::getHeight();
	const double circleSize  = (double) width / 32.0;
	const double halfSize    = circleSize / 2.0;
	const double shapeHeight = (double) height / 4.0;
	const double shapeWidth  = (double) width / 32.0;

	if (ballMove >= 0.5) {
		ballX += std::cos(ballAngle * radian) * ballSpeed * dt;
		ballY += std::sin(ballAngle * radian) * ballSpeed * dt;
	}

	double playerDiff = Astrum::math::max(ballY - player, player - ballY);
	if (ballX < shapeWidth && playerDiff <= shapeHeight && ballY <= player) {
		deflectBall(-1, 1, 0, velocityTransfer * playerVel);
		ballX = shapeWidth;
	} else if (ballX < halfSize) {
		computerScore++;
		initBall(0);
	}

	double computerDiff = Astrum::math::max(ballY - computer, computer - ballY);
	if (ballX > (width - shapeWidth) && computerDiff <= shapeHeight && ballY <= computer) {
		deflectBall(-1, 1, 0, velocityTransfer * computerVel);
		ballX = width - shapeWidth;
	} else if (ballX > width - halfSize) {
		playerScore++;
		initBall(1);
	}

	if (ballY < halfSize) {
		ballY = halfSize + 1;
		deflectBall(1, -1, 0, 0);
	} else if (ballY > height - halfSize) {
		ballY = height - halfSize - 1;
		deflectBall(1, -1, 0, 0);
	}
}

void update(double dt)
{
	if (Astrum::keyboard::isdown("escape"))
		Astrum::quit();

	if (ballMove < 0.5)
		ballMove += dt;

	double playerVel = updatePlayer(dt);
	double computerVel = updateComputer(dt);

	updateBall(dt, playerVel, computerVel);
}

void draw()
{
	const int width  = Astrum::window::getWidth();
	const int height = Astrum::window::getHeight();
	const double shapeHeight = (double) height / 4.0;
	const double shapeWidth  = (double) width / 32.0;
	const double circleSize  = (double) width / 32.0;
	const double lineHeight  = (double) height / 20.0;
	const double lineWidth   = (double) width / 64.0;

	Astrum::graphics::rectangle(0, player - shapeHeight, shapeWidth,
		shapeHeight, white, true);
	Astrum::graphics::rectangle(width - shapeWidth, computer - shapeHeight,
		shapeWidth, shapeHeight, white, true);
	Astrum::graphics::circle(ballX, ballY, circleSize, white, true);

	for (int i = 0; i < 10; i++) {
		Astrum::graphics::rectangleFilled((width - lineWidth) / 2,
			lineHeight * (i * 2 + 1), lineWidth, lineHeight, white);
	}

	std::string str = Astrum::util::strformat("Player: %d - Computer: %d",
		playerScore, computerScore);
	const char *cstr = str.c_str();
	int textWidth, textHeight;
	if (font == nullptr) {
		printf("howdy! score: %s\n", str.c_str());
		return;
	}
	std::tie(textWidth, textHeight) = font->textSize(cstr);
	Astrum::graphics::print(cstr, (width - textWidth) / 2, 5, white);
}

void startup()
{
	const int height = Astrum::window::getHeight();
	const double shapeHeight = (double) height / 4.0;
	const double center = (double) (height - shapeHeight) / 2.0 + shapeHeight;
	player = computer = center;
	playerScore = computerScore = 0;
	font = Astrum::graphics::font;
	initBall(0);
}

int main()
{
	Astrum::Config conf;
	conf.windowFullscreen = false;
	conf.windowResizable = true;
	conf.windowTitle = "Pong Test";

	Astrum::init(&conf);
	Astrum::graphics::setBackgroundColor(black);

	Astrum::onstartup(startup);
	Astrum::ondraw(draw);

	Astrum::run(update);

	Astrum::exit();

	return 0;
}
