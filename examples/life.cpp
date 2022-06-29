#include <astrum/astrum.hpp>

#include <array>

const int GAME_SIZE = 18;
const Astrum::Color white = Astrum::Color(0xFFFFFF);
const Astrum::Color black = Astrum::Color(0x000000);

using board = std::array<std::array<bool, GAME_SIZE>, GAME_SIZE>;

struct GameState {
	int tick;
	bool paused;
	board live;
	double nextTick;
	double tickRate = 0.1; // 10 ticks per second
};
GameState game;

void clear(GameState &game) {
	for (int i = 0; i < GAME_SIZE; i++) {
		for (int j = 0; j < GAME_SIZE; j++) {
			game.live[i][j] = false;
		}
	}
}

int adjacentLive(const board &live, int x, int y) {
	const int xLeft  = x == 0 ? 17 : x - 1;
	const int xRight = x == 17 ? 0 : x + 1;
	const int yUp    = y == 0 ? 17 : y - 1;
	const int yDown  = y == 17 ? 0 : y + 1;
	return live[xLeft][yUp] + live[xLeft][y] + live[xLeft][yDown]
		+ live[x][yUp] + live[x][yDown] + live[xRight][yUp]
		+ live[xRight][y] + live[xRight][yDown];
}

void tick(GameState &game) {
	board newLive;
	for (int i = 0; i < GAME_SIZE; i++) {
		for (int j = 0; j < GAME_SIZE; j++) {
			const int adjacent = adjacentLive(game.live, i, j);
			if (game.live[i][j]) {
				if (adjacent > 3 || adjacent < 2) {
					newLive[i][j] = false;
				} else {
					newLive[i][j] = true;
				}
			} else {
				if (adjacent == 3) {
					newLive[i][j] = true;
				} else {
					newLive[i][j] = false;
				}
			}
		}
	}
	game.live = newLive;
	game.tick++;
}

void reset(GameState &game) {
	clear(game);
	game.tick = 0;
	game.paused = true;
	game.nextTick = 0.0;
}

void startup() {
	reset(game);
}

void update(double dt) {
	if (Astrum::keyboard::isdown("escape"))
		Astrum::quit();

	if (game.paused)
		return;
	game.nextTick += dt;
	while (game.nextTick >= game.tickRate) {
		tick(game);
		game.nextTick -= game.tickRate;
	}
}

void keypress(Astrum::Key key) {
	if (key == Astrum::Key::SPACE)
		game.paused = !game.paused;
}

void draw() {
	for (int i = 0; i < GAME_SIZE; i++) {
		for (int j = 0; j < GAME_SIZE; j++) {
			Astrum::graphics::rectangle(30 * i + 1, 30 * j + 1, 28,
				28, game.live[i][j] ? white : black, true);
		}
	}
}

void mousepress(Astrum::MouseButton button, int x, int y) {
	if (button == Astrum::MouseButton::LEFT) {
		int xIndex = x / 30;
		int yIndex = y / 30;
		game.live[xIndex][yIndex] = !game.live[xIndex][yIndex];
	}
}

int main() {
	Astrum::Config conf;
	conf.windowFullscreen = false;
	conf.windowResizable = true;
	conf.appName = "Game of Life";
	conf.windowWidth = conf.minWindowWidth = 540;
	conf.windowHeight = conf.minWindowHeight = 540;
	conf.scaleToSize = true;

	Astrum::init(conf);
	Astrum::graphics::setBackgroundColor(black);
	Astrum::graphics::setColor(white);

	Astrum::onstartup(startup);
	Astrum::ondraw(draw);
	Astrum::onkeypressed(keypress);
	Astrum::onmousepressed(mousepress);

	Astrum::run(update);

	Astrum::exit();

	return 0;
}
