#include <astrum/astrum.hpp>

#include <cmath>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <random>
#include <memory>

// on macOS, use CMD for undo/redo
// otherwise, use CTRL
#if defined(__APPLE__) || defined(__MACH__)
	#define SHORTCUT_MOD Astrum::KeyMod::GUI
#else
	#define SHORTCUT_MOD Astrum::KeyMod::CTRL
#endif

const Astrum::Color white = Astrum::Color(255, 255, 255);
const Astrum::Color black = Astrum::Color(0, 0, 0);
const Astrum::Color  gray = Astrum::Color(128, 128, 128);
const Astrum::Color lgray = Astrum::Color(176, 176, 176);
const Astrum::Color  blue = Astrum::Color(85, 85, 255);
const Astrum::Color   red = Astrum::Color(255, 0, 0);

std::shared_ptr<Astrum::Font> bigFont, smallFont;

const int boxStart[9] = { 0, 3, 6, 27, 30, 33, 54, 57, 60 };

const int boxSize = 41;
const int bigLineSize = 3;
const int gridOffset = 10;
const int gridSize = boxSize * 9 + bigLineSize * 4;

enum Difficulty { EASY, MEDIUM, HARD };

enum PuzzleEventType {
	DELETE,
	WRITE_1, WRITE_2, WRITE_3, WRITE_4, WRITE_5, WRITE_6, WRITE_7, WRITE_8,
	WRITE_9,
	TGL_NOTE_1, TGL_NOTE_2, TGL_NOTE_3, TGL_NOTE_4, TGL_NOTE_5, TGL_NOTE_6,
	TGL_NOTE_7, TGL_NOTE_8, TGL_NOTE_9,
	CLEAR_NOTES
};

struct PuzzleEvent {
	int x, y;
	short original;
	PuzzleEventType type;
};

struct {
	int x, y;

	short problem[81];
	short notes[81];
	bool fixed[81];

	double time;

	size_t eventPointer;
	size_t eventLength;
	size_t redoLength;
	std::vector<PuzzleEvent> eventStack;

	bool won;
	bool quit;
} puzzle;

struct {
	bool showErrors = true;
	Difficulty difficulty = MEDIUM;
	bool paused = false;
	bool notes = false;
	bool changedDifficulty = false;
	double givingUp = 1.0;
} puzzleConfig;

class Button
{
	std::string text;
	float x;
	float y;
	int textWidth;
	int textHeight;
	std::function<void()> onclick;
	bool mouseDown;
public:
	Button(std::string text, float x, float y, std::function<void()> onclick)
		: text(text), x(x), y(y), onclick(onclick), mouseDown(false)
	{
		Astrum::graphics::getFont()->textSize(text, textWidth, textHeight);
	}
	void draw(bool shaded = false)
	{
		if (shaded || mouseDown) {
			Astrum::graphics::rectangleFilled(x, y, textWidth + 4,
				textHeight + 4, lgray);
		}
		Astrum::graphics::rectangle(x, y, textWidth + 4, textHeight + 4, black);
		Astrum::graphics::print(text, x + 2, y + 2);
	}
	void update(bool disabled = false)
	{
		if (disabled == true)
			return;
		int mouseX, mouseY;
		std::tie(mouseX, mouseY) = Astrum::mouse::getCoordinates();
		if (Astrum::mouse::isdown(Astrum::MouseButton::LEFT) && !mouseDown
		&& (mouseX >= x && mouseX <= x + textWidth + 4)
		&& (mouseY >= y && mouseY <= y + textHeight + 4)) {
			mouseDown = true;
			onclick();
		} else if (!Astrum::mouse::isdown(Astrum::MouseButton::LEFT)) {
			mouseDown = false;
		}
	}
};

Button *newGameButton;
Button *easyButton;
Button *mediumButton;
Button *hardButton;
Button *notesButton;
Button *solveButton;
Button *undoButton;
Button *redoButton;
Button *pauseButton;

int randint(int low, int high)
{
	return Astrum::math::random(low, high - 1);
}

void shuffle(short arr[], int n)
{
	for (int i = n - 1; i > 0; i--) {
		int j = randint(0, i + 1);
		short tmp = arr[j];
		arr[j] = arr[i];
		arr[i] = tmp;
	}
}

bool containsDup(short row[], int n)
{
	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++) {
			if (row[i] == row[j] && row[i] != 0)
				return true;
		}
	}
	return false;
}

bool checkCorrectness(short sudoku[81])
{
	short row[9] = { 0 };
	short col[9] = { 0 };
	short box[9] = { 0 };

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			int boxIdx = boxStart[i] + (j / 3) * 9 + j % 3;

			row[j] = sudoku[i * 9 + j];
			col[j] = sudoku[i + j * 9];
			box[j] = sudoku[boxIdx];
		}

		if (containsDup(row, 9) || containsDup(col, 9) || containsDup(box, 9))
			return false;
	}
	return true;
}

bool isSolution(short sudoku[81])
{
	for (int i = 0; i < 81; i++) {
		if (sudoku[i] == 0)
			return false;
	}
	return checkCorrectness(sudoku);
}

bool duplicateNumber(short sudoku[81], int x, int y, short num)
{
	if (num == 0)
		return false;

	int boxNo = 3 * (y / 3) + (x / 3);

	for (int i = 0; i < 9; i++) {
		int boxIdx = boxStart[boxNo] + (i / 3) * 9 + i % 3;

		int row = i == x ? 0 : sudoku[y * 9 + i];
		int col = i == y ? 0 : sudoku[x + i * 9];
		int box = boxIdx == x + y * 9 ? 0 : sudoku[boxIdx];

		if (row == num || col == num || box == num)
			return true;
	}
	return false;
}

bool solve(short sudoku[81], int x, int y) {
	if (y == 9)
		return true;
	else if (x == 9)
		return solve(sudoku, 0, y + 1);
	if (sudoku[x + y * 9] != 0)
		return solve(sudoku, x + 1, y);
	for (int possible = 1; possible <= 9; possible++) {
		if (!duplicateNumber(sudoku, x, y, possible)) {
			sudoku[x + y * 9] = possible;
			if (solve(sudoku, x + 1, y))
				return true;
			sudoku[x + y * 9] = 0;
		}
	}
	return false;
}
bool solve(short sudoku[81]) {
	return solve(sudoku, 0, 0);
}

void generate(short sudoku[81], Difficulty difficulty)
{
	short topcorner[9]    = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	short middle[9]       = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	short bottomcorner[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	shuffle(topcorner, 9);
	shuffle(middle, 9);
	shuffle(bottomcorner, 9);

	for (int i = 0; i < 9; i++) {
		int offset = (i / 3) * 9 + i % 3;
		sudoku[boxStart[0] + offset] = topcorner[i];
		sudoku[boxStart[4] + offset] = middle[i];
		sudoku[boxStart[8] + offset] = bottomcorner[i];
	}

	solve(sudoku);

	int boxesToDrop = 0;
	switch (difficulty) {
		case EASY: boxesToDrop = 30; break;
		case MEDIUM: boxesToDrop = 40; break;
		case HARD: boxesToDrop = 50; break;
	}

	short drop, counter;
	for (int i = 0; i < boxesToDrop; i++) {
		drop = randint(0, 81);
		counter = 0;
		while (sudoku[drop] == 0 && counter++ < 10)
			drop = randint(0, 81);
		sudoku[drop] = 0;
	}
}

bool addNumber(int x, int y, short num)
{
	int pos = x + y * 9;
	if (puzzle.fixed[pos])
		return false;
	puzzle.problem[pos] = num;
	return true;
}

short getNumber(int x, int y)
{
	return puzzle.problem[x + y * 9];
}

bool tglNote(int x, int y, short note)
{
	int pos = x + y * 9;
	if (puzzle.fixed[pos] || puzzle.problem[pos])
		return false;
	puzzle.notes[pos] ^= 1 << (note - 1);
	return true;
}

bool setNote(int x, int y, short note)
{
	int pos = x + y * 9;
	if (puzzle.fixed[pos] || puzzle.problem[pos])
		return false;
	puzzle.notes[pos] = note;
	return true;
}

short getNote(int x, int y)
{
	return puzzle.notes[x + y * 9];
}

bool handleEvent(PuzzleEvent &event)
{
	puzzle.x = event.x;
	puzzle.y = event.y;

	switch (event.type) {
	case DELETE:
		event.original = getNumber(event.x, event.y);
		return addNumber(event.x, event.y, 0);
	case WRITE_1:
	case WRITE_2:
	case WRITE_3:
	case WRITE_4:
	case WRITE_5:
	case WRITE_6:
	case WRITE_7:
	case WRITE_8:
	case WRITE_9:
		event.original = getNumber(event.x, event.y);
		return addNumber(event.x, event.y, event.type - WRITE_1 + 1);
	case TGL_NOTE_1:
	case TGL_NOTE_2:
	case TGL_NOTE_3:
	case TGL_NOTE_4:
	case TGL_NOTE_5:
	case TGL_NOTE_6:
	case TGL_NOTE_7:
	case TGL_NOTE_8:
	case TGL_NOTE_9:
		event.original = getNote(event.x, event.y);
		return tglNote(event.x, event.y, event.type - TGL_NOTE_1 + 1);
	case CLEAR_NOTES:
		event.original = getNote(event.x, event.y);
		return setNote(event.x, event.y, 0);
	}
}

void undoEvent(PuzzleEvent event)
{
	puzzle.x = event.x;
	puzzle.y = event.y;

	switch (event.type) {
	case DELETE:
	case WRITE_1:
	case WRITE_2:
	case WRITE_3:
	case WRITE_4:
	case WRITE_5:
	case WRITE_6:
	case WRITE_7:
	case WRITE_8:
	case WRITE_9:
		addNumber(event.x, event.y, event.original);
		break;
	case TGL_NOTE_1:
	case TGL_NOTE_2:
	case TGL_NOTE_3:
	case TGL_NOTE_4:
	case TGL_NOTE_5:
	case TGL_NOTE_6:
	case TGL_NOTE_7:
	case TGL_NOTE_8:
	case TGL_NOTE_9:
	case CLEAR_NOTES:
		setNote(event.x, event.y, event.original);
		break;
	}
}

void printNumber(int x, int y, short num, Astrum::Color color, std::shared_ptr<Astrum::Font> font)
{
	std::string str = std::to_string(num);
	int textWidth, textHeight;
	std::tie(textWidth, textHeight) = font->textSize(str);
	Astrum::graphics::print(str, x - (textWidth / 2), y - (textHeight / 2), font, color);
}

void printGrid(short sudoku[81], short notes[81], int selectedX, int selectedY, bool showErrors)
{
	for (int i = 0; i < 81; i++) {
		int x = gridOffset + boxSize * (i % 9) + bigLineSize * ((i % 9) / 3 + 1);
		int y = gridOffset + boxSize * (i / 9) + bigLineSize * ((i / 27) + 1);
		Astrum::graphics::rectangle(x, y, boxSize, boxSize, black);

		short num = sudoku[i];
		short note = notes[i];
		if (num != 0) {
			Astrum::Color color = puzzle.fixed[i] ? blue : black;
			if (showErrors && duplicateNumber(sudoku, i % 9, i / 9, num))
				color = red;
			if (puzzle.won || puzzle.quit)
				color = blue;
			printNumber(x + boxSize / 2, y + boxSize / 2, num, color, bigFont);
		} else if (note != 0) {
			int k = 0;
			while (note > 0) {
				if (note & 1) {
					int offsetX = (k % 3 + 1) * (boxSize / 4);
					int offsetY = (k / 3 + 1) * (boxSize / 4);
					printNumber(x + offsetX, y + offsetY, k + 1, gray, smallFont);
				}
				note >>= 1;
				k++;
			}
		}
	}

	int x = gridOffset + boxSize * selectedX + bigLineSize * (selectedX / 3 + 1);
	int y = gridOffset + boxSize * selectedY + bigLineSize * (selectedY / 3 + 1);
	Astrum::graphics::rectangleFilled(x - 1, y, 2, boxSize, blue);
	Astrum::graphics::rectangleFilled(x + boxSize - 1, y, 2, boxSize, blue);
	Astrum::graphics::rectangleFilled(x, y - 1, boxSize, 2, blue);
	Astrum::graphics::rectangleFilled(x, y + boxSize - 1, boxSize, 2, blue);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int x = gridOffset + boxSize * i * 3 + bigLineSize * i;
			int y = gridOffset + boxSize * i * 3 + bigLineSize * i;
			Astrum::graphics::rectangleFilled(x, gridOffset,
				bigLineSize, gridSize, black);
			Astrum::graphics::rectangleFilled(gridOffset, y,
				gridSize, bigLineSize, black);
		}
	}
}

void startup()
{
	puzzle.x = puzzle.y = 4;

	memset(&puzzle.problem, 0, 81 * sizeof(short));
	generate(puzzle.problem, puzzleConfig.difficulty);
	for (int i = 0; i < 81; i++)
		puzzle.fixed[i] = puzzle.problem[i] != 0;
	puzzleConfig.changedDifficulty = false;

	puzzle.eventPointer = 0;
	puzzle.redoLength = 0;
	puzzle.eventLength = 0;
	puzzle.eventStack.clear();

	puzzle.time = 0.0;
	puzzle.won = false;
	puzzle.quit = false;

	memset(&puzzle.notes, 0, 81 * sizeof(short));
}

std::string prettyTime(double seconds)
{
	std::ostringstream out;
	out.precision(2);
	if (seconds > 60) {
		int minutes = seconds / 60;
		if (minutes > 60) {
			int hours = minutes / 60;
			out << std::to_string(hours) << "h";
			minutes %= 60;
			seconds -= (hours * 3600.0);
		}
		out << std::to_string(minutes) << "m";
		seconds -= (minutes * 60.0);
		out.precision(0);
	}
	out << std::fixed << seconds << "s";
	return out.str();
}

void draw()
{
	if (puzzleConfig.paused) {
		int width = 9 * boxSize + 3 * bigLineSize;
		Astrum::graphics::rectangle(gridOffset, gridOffset, width, width, black);
		int textWidth, textHeight;
		std::tie(textWidth, textHeight) = bigFont->textSize("Paused");
		Astrum::graphics::print("Paused", gridOffset + (width / 2) - (textWidth / 2),
			gridOffset + (width / 2) - (textHeight / 2), bigFont, black);
	} else {
		printGrid(puzzle.problem, puzzle.notes, puzzle.x, puzzle.y,
			puzzleConfig.showErrors);
	}

	std::string formattedText;
	std::string formattedTime = prettyTime(puzzle.time);
	if (puzzle.won) {
		// To-do: show "You finished the puzzle!" in black over the puzzle
		formattedTime = Astrum::util::strformat("Time to complete: %s",
			formattedTime.c_str());
		Astrum::graphics::print(formattedTime,
			gridSize + gridOffset * 2, gridOffset);
	} else if (puzzle.quit) {
		formattedTime = Astrum::util::strformat("Quit after %s",
			formattedTime.c_str());
		Astrum::graphics::print(formattedTime,
			gridSize + gridOffset * 2, gridOffset);
	} else {
		formattedTime = Astrum::util::strformat("Time: %s",
			formattedTime.c_str());
		Astrum::graphics::print(formattedTime,
			gridSize + gridOffset * 2, gridOffset);
	}

	newGameButton->draw();

	easyButton->draw(puzzleConfig.difficulty == EASY);
	mediumButton->draw(puzzleConfig.difficulty == MEDIUM);
	hardButton->draw(puzzleConfig.difficulty == HARD);
	if (puzzleConfig.changedDifficulty) {
		Astrum::graphics::print("You changed the difficulty. For this to take",
			gridSize + gridOffset * 2, 70 + gridOffset * 2,
			smallFont);
		Astrum::graphics::print("effect, you need to start a new puzzle.",
			gridSize + gridOffset * 2, 80 + gridOffset * 2,
			smallFont);
	}

	notesButton->draw(puzzleConfig.notes);

	Astrum::graphics::print("To give up and solve the puzzle, click",
		gridSize + gridOffset * 2, 120 + gridOffset * 2,
		smallFont);
	Astrum::graphics::print("the button twice. Pressing the button",
		gridSize + gridOffset * 2, 130 + gridOffset * 2,
		smallFont);
	Astrum::graphics::print("while it's shaded will solve the puzzle.",
		gridSize + gridOffset * 2, 140 + gridOffset * 2,
		smallFont);
	solveButton->draw(puzzleConfig.givingUp < 0.4);

	undoButton->draw(puzzle.eventPointer == 0);
	redoButton->draw(puzzle.redoLength == 0);

	pauseButton->draw(puzzleConfig.paused);
}

void update(double dt)
{
	if (Astrum::keyboard::isdown("escape"))
		Astrum::quit();

	newGameButton->update();
	easyButton->update();
	mediumButton->update();
	hardButton->update();
	notesButton->update();
	solveButton->update();
	undoButton->update(puzzle.eventPointer == 0);
	redoButton->update(puzzle.redoLength == 0);
	pauseButton->update();

	puzzleConfig.givingUp += dt;

	if (puzzleConfig.paused || puzzle.won || puzzle.quit)
		return;

	puzzle.time += dt;

	while (puzzle.eventPointer < puzzle.eventLength) {
		PuzzleEvent ev = puzzle.eventStack.at(puzzle.eventPointer);
		bool success = handleEvent(ev);
		if (success) {
			puzzle.eventPointer++;
		} else {
			puzzle.eventLength--;
		}
		if (isSolution(puzzle.problem))
			puzzle.won = true;
	}
}

void pushEvent(int x, int y, PuzzleEventType kind)
{
	PuzzleEvent event = { .x = x, .y = y, .original = 0, .type = kind };
	if (puzzle.redoLength) {
		puzzle.eventStack.erase(puzzle.eventStack.begin()
			+ puzzle.eventLength, puzzle.eventStack.end());
		puzzle.redoLength = 0;
	}
	if (puzzle.eventStack.size() > puzzle.eventLength) {
		puzzle.eventStack[puzzle.eventLength] = event;
	} else {
		puzzle.eventStack.push_back(event);
	}
	puzzle.eventLength++;
}

void redo()
{
	if (puzzle.redoLength) {
		puzzle.eventLength++;
		puzzle.redoLength--;
	}
}

void undo()
{
	if (puzzle.eventPointer) {
		puzzle.eventPointer--;
		undoEvent(puzzle.eventStack.at(puzzle.eventPointer));
		puzzle.eventLength--;
		puzzle.redoLength++;
	}
}

void keypress(Astrum::Key key, Astrum::KeyMod mod)
{
	if (key == Astrum::Key::SPACE)
		puzzleConfig.paused = !puzzleConfig.paused;

	if (puzzleConfig.paused)
		return;

	if (key == Astrum::Key::UP)
		puzzle.y -= 1;
	else if (key == Astrum::Key::DOWN)
		puzzle.y += 1;
	else if (key == Astrum::Key::LEFT)
		puzzle.x -= 1;
	else if (key == Astrum::Key::RIGHT)
		puzzle.x += 1;
	puzzle.x = puzzle.x > 8 ? 8 : puzzle.x < 0 ? 0 : puzzle.x;
	puzzle.y = puzzle.y > 8 ? 8 : puzzle.y < 0 ? 0 : puzzle.y;

	if (key >= Astrum::Key::ONE && key <= Astrum::Key::NINE) {
		PuzzleEventType ev = (PuzzleEventType) ((puzzleConfig.notes
			? TGL_NOTE_1 : WRITE_1) + int(key) - int(Astrum::Key::ONE));
		pushEvent(puzzle.x, puzzle.y, ev);
	} if (key == Astrum::Key::BACKSPACE) {
		PuzzleEventType ev = puzzleConfig.notes ? CLEAR_NOTES : DELETE;
		pushEvent(puzzle.x, puzzle.y, ev);
	}

	if (key == Astrum::Key::N && (mod & SHORTCUT_MOD)) {
		// CTRL+N = new puzzle
		startup();
	} else if (key == Astrum::Key::N) {
		// N = toggle notes mode
		puzzleConfig.notes = !puzzleConfig.notes;
	}

	if (key == Astrum::Key::Z && (mod & SHORTCUT_MOD))
		// CTRL+Z = undo
		undo();
	else if (key == Astrum::Key::Y && (mod & SHORTCUT_MOD))
		// CTRL+Y = redo
		redo();
}

void mousepress(Astrum::MouseButton button, int x, int y)
{
	if (puzzleConfig.paused)
		return;

	if (button == Astrum::MouseButton::LEFT) {
		if ((x >= gridOffset && x <= gridOffset + gridSize)
		&& (y >= gridOffset && y <= gridOffset + gridSize)) {
			x = x - gridOffset - bigLineSize;
			y = y - gridOffset - bigLineSize;
			int selectedX = 0;
			int selectedY = 0;
			while (x > 3 * boxSize) {
				x = x - bigLineSize - 3 * boxSize;
				selectedX += 3;
			}
			while (y > 3 * boxSize) {
				y = y - bigLineSize - 3 * boxSize;
				selectedY += 3;
			}
			selectedX += x / boxSize;
			selectedY += y / boxSize;
			puzzle.x = selectedX > 8 ? 8 : selectedX < 0 ? 0 : selectedX;
			puzzle.y = selectedY > 8 ? 8 : selectedY < 0 ? 0 : selectedY;
		}
	}
}

int main()
{
	Astrum::Config conf;
	conf.windowFullscreen = false;
	conf.windowResizable = true;
	conf.appName = "Sudoku";
	conf.minWindowWidth = 640;
	conf.minWindowHeight = 480;
	conf.scaleToSize = true;

	Astrum::init(conf);
	Astrum::graphics::setBackgroundColor(white);
	bigFont = std::make_shared<Astrum::Font>(24);
	smallFont = std::make_shared<Astrum::Font>(10);
	Astrum::keyboard::setKeyRepeat(true);

	newGameButton = new Button("New Game", gridSize + gridOffset * 2,
		12 + gridOffset * 2, startup);
	easyButton = new Button("Easy", gridSize + gridOffset * 2,
		42 + gridOffset * 2, [&]() {
			puzzleConfig.difficulty = EASY;
			puzzleConfig.changedDifficulty = true;
		});
	mediumButton = new Button("Medium", gridSize + gridOffset * 2 + 45,
		42 + gridOffset * 2, [&]() {
			puzzleConfig.difficulty = MEDIUM;
			puzzleConfig.changedDifficulty = true;
		});
	hardButton = new Button("Hard", gridSize + gridOffset * 2 + 120,
		42 + gridOffset * 2, [&]() {
			puzzleConfig.difficulty = HARD;
			puzzleConfig.changedDifficulty = true;
		});
	notesButton = new Button("Edit Notes", gridSize + gridOffset * 2,
		96 + gridOffset * 2, [&]() {
			puzzleConfig.notes = !puzzleConfig.notes;
		});
	solveButton = new Button("Give Up", gridSize + gridOffset * 2,
		156 + gridOffset * 2, [&]() {
			if (puzzleConfig.givingUp < 0.4) {
				for (int i = 0; i < 81; i++)
					if (!puzzle.fixed[i])
						puzzle.problem[i] = 0;
				solve(puzzle.problem);
				puzzle.quit = true;
			}
			puzzleConfig.givingUp = 0;
		});
	undoButton = new Button("Undo", gridSize + gridOffset * 2,
		190 + gridOffset * 2, undo);
	redoButton = new Button("Redo", gridSize + gridOffset * 2 + 50,
		190 + gridOffset * 2, redo);
	pauseButton = new Button("Pause", gridSize + gridOffset * 2,
		226 + gridOffset * 2, [&]() {
			puzzleConfig.paused = !puzzleConfig.paused;
		});

	Astrum::onstartup(startup);
	Astrum::ondraw(draw);
	Astrum::onkeypressed(keypress);
	Astrum::onmousepressed(mousepress);

	Astrum::run(update);

	Astrum::exit();

	return 0;
}
