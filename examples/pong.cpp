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

void initBall(int lastScore)
{
	const int width  = Astrum::window::getWidth();
	const int height = Astrum::window::getHeight();
	ballAngle = ((lastScore ? 195 : 15) + Astrum::math::random(4) * 15.0) * radian;
	ballX = (double) width / 2.0;
	ballY = (double) height / 2.0;
	ballMove = 0.0;
	ballSpeed = width * 2.0 / 3.0;
	ballPow = 1.0;
}

void deflectBall(double xMod, double yMod, double xAdd, double yAdd)
{
	double ballVx = std::cos(ballAngle) * ballSpeed * xMod + xAdd;
	double ballVy = std::sin(ballAngle) * ballSpeed * yMod + yAdd;
	ballAngle = std::atan2(ballVy, ballVx);
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
		playerDir += 270.0 * radian;
	}
	if (Astrum::keyboard::isdown("up") || Astrum::keyboard::isdown("w")) {
		player -= speed * dt;
		playerDir += 90.0 * radian;
	}

	if (player < shapeHeight)
		player = shapeHeight;
	else if (player > height)
		player = height;

	return speed * std::cos(playerDir);
}

double updateComputer(double dt)
{
	const int height = Astrum::window::getHeight();
	const double shapeHeight = (double) height / 4.0;
	const double speed = (double) height;

	double compDir = 0.0;
	if (computer - shapeHeight / 2.0 < ballY) {
		computer += speed * dt;
		compDir += 270.0 * radian;
	} else if (computer - shapeHeight / 2.0 > ballY) {
		computer -= speed * dt;
		compDir += 90.0 * radian;
	}

	if (computer < shapeHeight)
		computer = shapeHeight;
	else if (computer > height)
		computer = height;

	return speed * std::cos(compDir);
}

bool ballCollision(double charY, bool end)
{
	const int width  = Astrum::window::getWidth();
	const int height = Astrum::window::getHeight();
	const double circleSize  = (double) width / 32.0;
	const double halfSize    = circleSize / 2.0;
	const double shapeHeight = (double) height / 4.0;
	const double shapeWidth  = (double) width / 32.0;

	double closestX, closestY;
	if (end && ballX > width)
		closestX = width;
	else if (!end && ballX < 0)
		closestX = 0;
	else if (end && ballX < width - shapeWidth)
		closestX = width - shapeWidth;
	else if (!end && ballX > shapeWidth)
		closestX = shapeWidth;
	else
		closestX = ballX;

	if (ballY > charY)
		closestY = charY;
	else if (ballY < charY - shapeHeight)
		closestY = charY - shapeHeight;
	else
		closestY = ballY;

	double xDiff = closestX - ballX;
	double yDiff = closestY - ballY;
	double distance2 = xDiff * xDiff + yDiff * yDiff;
	return distance2 <= halfSize * halfSize;
}

void updateBall(double dt, double playerVel, double computerVel)
{
	const int width  = Astrum::window::getWidth();
	const int height = Astrum::window::getHeight();
	const double circleSize  = (double) width / 32.0;
	const double halfSize    = circleSize / 2.0;
	const double shapeWidth  = (double) width / 32.0;

	if (ballMove < 0.5) {
		ballMove += dt;
	} else {
		ballX += std::cos(ballAngle) * ballSpeed * dt;
		ballY += std::sin(ballAngle) * ballSpeed * dt;
	}

	if (ballCollision(player, false)) {
		deflectBall(-1, 1, 0, velocityTransfer * playerVel);
		ballX = shapeWidth + halfSize;
	} else if (ballCollision(computer, true)) {
		deflectBall(-1, 1, 0, velocityTransfer * computerVel);
		ballX = width - shapeWidth - halfSize;
	} else if (ballX < halfSize) {
		computerScore++;
		initBall(0);
	} else if (ballX > width - halfSize) {
		playerScore++;
		initBall(1);
	}

	if (ballY < circleSize) {
		ballY = circleSize + 1;
		deflectBall(1, -1, 0, 0);
	} else if (ballY > height - circleSize) {
		ballY = height - circleSize - 1;
		deflectBall(1, -1, 0, 0);
	}
}

void update(double dt)
{
	if (Astrum::keyboard::isdown("escape"))
		Astrum::quit();

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

	Astrum::graphics::rectangleFilled(0, player - shapeHeight, shapeWidth,
		shapeHeight, white);
	Astrum::graphics::rectangleFilled(width - shapeWidth,
		computer - shapeHeight, shapeWidth, shapeHeight, white);
	Astrum::graphics::circleFilled(ballX, ballY, circleSize, white);

	for (int i = 0; i < 10; i++) {
		Astrum::graphics::rectangleFilled((width - lineWidth) / 2,
			lineHeight * (i * 2 + 1), lineWidth, lineHeight, white);
	}

	Astrum::Font *font = Astrum::graphics::getFont();
	std::string str = Astrum::util::strformat("Player: %d - Computer: %d",
		playerScore, computerScore);
	const char *cstr = str.c_str();
	int textWidth, textHeight;
	if (font == nullptr) {
		printf("howdy! score: %s\n", cstr);
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
