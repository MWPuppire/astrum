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
int ballCollisions;
const double radian = M_PI / 180.0;
const double velocityTransfer = 0.5;
const double ballCollisionSpeed = 1.03;

void initBall()
{
	const int width  = Astrum::window::getWidth();
	const int height = Astrum::window::getHeight();
	ballAngle = (lastScore ? 195 : 15) + Astrum::math::random(4) * 15.0;
	ballX = (double) width / 2.0;
	ballY = (double) height / 2.0;
	ballMove = 0.0;
	ballSpeed = width;
	ballCollisions = 0;
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
	ballSpeed = Astrum::window::getWidth() * std::pow(ballCollisionSpeed, ++ballCollisions);
}

void update(double dt)
{
	if (Astrum::keyboard::isdown("escape"))
		Astrum::quit();

	const int width  = Astrum::window::getWidth();
	const int height = Astrum::window::getHeight();
	const double circleSize  = (double) width / 24.0;
	const double halfSize    = circleSize / 2.0;
	const double shapeHeight = (double) height / 4.0;
	const double shapeWidth  = (double) width / 32.0;
	const double speed = (double) height;

	if (ballMove < 0.5)
		ballMove += dt;

	double playerDir = 0.0;
	if (Astrum::keyboard::isdown("down") || Astrum::keyboard::isdown("s")) {
		player += speed * dt;
		playerDir += 270.0;
	}
	if (Astrum::keyboard::isdown("up") || Astrum::keyboard::isdown("w")) {
		player -= speed * dt;
		playerDir += 90.0;
	}
	player = (player < shapeHeight ? shapeHeight : (player > height ? height : player));
	double compDir = 0.0;
	if (computer - shapeHeight / 2.0 < ballY) {
		computer += speed * dt;
		compDir += 270.0;
	} else if (computer - shapeHeight / 2.0 > ballY) {
		computer -= speed * dt;
		compDir += 90.0;
	}
	computer = (computer < shapeHeight ? shapeHeight : (computer > height ? height : computer));

	if (ballMove >= 0.5) {
		ballX += std::cos(ballAngle * radian) * ballSpeed * dt;
		ballY += std::sin(ballAngle * radian) * ballSpeed * dt;
	}
	double playerDiff = Astrum::math::max(ballY - player, player - ballY);
	if (ballX < shapeWidth && playerDiff <= shapeHeight && ballY <= player) {
		double playerVel = speed * std::cos(playerDir * radian);
		deflectBall(-1, 1, 0, velocityTransfer * playerVel);
		ballX = shapeWidth;
	} else if (ballX < halfSize) {
		computerScore++;
		lastScore = 0;
		initBall();
	}
	double computerDiff = Astrum::math::max(ballY - computer, computer - ballY);
	if (ballX > (width - shapeWidth) && computerDiff <= shapeHeight && ballY <= computer) {
		double computerVel = speed * std::cos(compDir * radian);
		deflectBall(-1, 1, 0, velocityTransfer * computerVel);
		ballX = width - shapeWidth;
	} else if (ballX > width - halfSize) {
		playerScore++;
		lastScore = 1;
		initBall();
	}
	if (ballY < halfSize) {
		ballY = halfSize + 1;
		deflectBall(1, -1, 0, 0);
	} else if (ballY > height - halfSize) {
		ballY = height - halfSize - 1;
		deflectBall(1, -1, 0, 0);
	}
}

void draw()
{
	const int width  = Astrum::window::getWidth();
	const int height = Astrum::window::getHeight();
	const double shapeHeight = (double) height / 4.0;
	const double shapeWidth  = (double) width / 32.0;
	const double circleSize  = (double) width / 24.0;

	Astrum::graphics::rectangle(0, player - shapeHeight, shapeWidth, shapeHeight, white, true);
	Astrum::graphics::rectangle(width - shapeWidth, computer - shapeHeight, shapeWidth, shapeHeight, white, true);
	Astrum::graphics::circle(ballX, ballY, circleSize, white, true);

	std::ostringstream scorestream;
	scorestream << "Player: " << playerScore << " - " << "Computer: " << computerScore;
	std::string str = scorestream.str();
	const char *toprint = str.c_str();
	int textWidth, textHeight;
	Astrum::Font *font = Astrum::graphics::getFont();
	font->textSize(toprint, &textWidth, &textHeight);
	Astrum::graphics::print(toprint, (width - textWidth) / 2, 5, white);
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
	initBall();
}

int main()
{
	Astrum::Config conf;
	conf.windowFullscreen = false;
	conf.windowResizable = true;
	conf.windowTitle = "Pong Test";

	Astrum::Init(&conf);

	Astrum::onstartup(startup);
	Astrum::ondraw(draw);

	Astrum::run(update);

	Astrum::Close();

	return 0;
}
