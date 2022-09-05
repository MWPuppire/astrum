#include <astrum/astrum.hpp>

#include <vector>

const double TICK_RATE = 0.2; // 5 ticks per second
const int GAME_SIZE = 12;
const Astrum::Color GREEN = Astrum::Color(0x00FF00);
const Astrum::Color RED   = Astrum::Color(0xFF0000);
const Astrum::Color BLACK = Astrum::Color(0x000000);
const Astrum::Color WHITE = Astrum::Color(0xFFFFFF);

enum Direction { UP, DOWN, LEFT, RIGHT };
struct Position {
	int x;
	int y;
	Position() : x(0), y(0) { }
	Position(int x, int y) : x(x), y(y) { }
	Position operator+(const Position &other) const {
		return Position(this->x + other.x, this->y + other.y);
	}
	bool operator==(const Position &other) const {
		return this->x == other.x && this->y == other.y;
	}
};
const Position MOVE_BY_DIRECTION[] = {
	Position( 0, -1), // UP
	Position( 0,  1), // DOWN
	Position(-1,  0), // LEFT
	Position( 1,  0), // RIGHT
};

struct GameState {
	Position food;
	std::vector<Position> snake;
	Direction facing;
	double tick;
	bool paused;
	int score;
	bool dead;
};
GameState game;

void reset(GameState &game) {
	game.snake = std::vector<Position>{ { 5, 3 }, { 5, 4 }, { 5, 5 } };
	game.facing = UP;
	game.score = 0;
	game.paused = false;
	game.tick = 0.0;
	game.dead = false;
	game.food = Position(Astrum::math::random(GAME_SIZE - 1),
		Astrum::math::random(GAME_SIZE - 1));
}

void tick(GameState &game) {
	auto &snake = game.snake;
	Position head = snake.front();
	Position tail = snake.back();
	Position newHead = head + MOVE_BY_DIRECTION[game.facing];
	if (head.y < 0 || head.x < 0 || head.y >= GAME_SIZE || head.x >= GAME_SIZE) {
		game.dead = true;
		return;
	}
	snake.insert(snake.begin(), newHead);
	for (size_t i = 1; i < snake.size(); i++) {
		Position &pos = snake[i];
		if (newHead == pos) {
			game.dead = true;
			return;
		}
	}
	snake.pop_back();
	if (newHead == game.food) {
		game.score += 1;
		snake.push_back(tail);
		Position newFood;
		while (1) {
			newFood = Position(
				Astrum::math::random(GAME_SIZE - 1),
				Astrum::math::random(GAME_SIZE - 1));
			bool inSnake = false;
			for (auto &pos : snake) {
				if (newFood == pos) {
					inSnake = true;
					break;
				}
			}
			if (!inSnake) {
				break;
			}
		}
		game.food = newFood;
	}
}

void startup() {
	reset(game);
}

void update(double dt) {
	if (Astrum::keyboard::isdown("escape"))
		Astrum::quit();

	if (game.paused)
		return;
	game.tick += dt;
	while (game.tick >= TICK_RATE) {
		tick(game);
		game.tick -= TICK_RATE;
	}
}

void keypress(Astrum::Key key) {
	if (key == Astrum::Key::SPACE) {
		if (game.dead)
			reset(game);
		else
			game.paused = !game.paused;
		return;
	}
	bool moved = false;
	if (key == Astrum::Key::LEFT && !moved) {
		if (game.facing == UP || game.facing == DOWN) {
			game.facing = LEFT;
			moved = true;
		}
	}
	if (key == Astrum::Key::RIGHT && !moved) {
		if (game.facing == UP || game.facing == DOWN) {
			game.facing = RIGHT;
			moved = true;
		}
	}
	if (key == Astrum::Key::UP && !moved) {
		if (game.facing == LEFT || game.facing == RIGHT) {
			game.facing = UP;
			moved = true;
		}
	}
	if (key == Astrum::Key::DOWN && !moved) {
		if (game.facing == LEFT || game.facing == RIGHT) {
			game.facing = DOWN;
			moved = true;
		}
	}
}

void draw() {
	if (game.dead) {
		Astrum::graphics::print("You died", 10, 10, WHITE);
		std::string formatted =
			Astrum::util::strformat("Score: %d", game.score);
		Astrum::graphics::print(formatted, 10, 40, WHITE);
	} else {
		for (auto &pos : game.snake) {
			Astrum::graphics::rectangle(30 * pos.x + 1, 30 * pos.y + 1,
				28, 28, GREEN, true);
		}
		Astrum::graphics::rectangle(30 * game.food.x + 1, 30 * game.food.y + 1,
			28, 28, RED, true);
	}
}

int main() {
	Astrum::Config conf;
	conf.windowFullscreen = false;
	conf.windowResizable = true;
	conf.appName = "Snake";
	conf.windowWidth = conf.minWindowWidth = 360;
	conf.windowHeight = conf.minWindowHeight = 360;
	conf.scaleToSize = true;

	Astrum::init(conf);
	Astrum::graphics::setBackgroundColor(BLACK);

	Astrum::onstartup(startup);
	Astrum::ondraw(draw);
	Astrum::onkeypressed(keypress);

	Astrum::run(update);

	Astrum::exit();

	return 0;
}
