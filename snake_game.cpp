#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>

using namespace std;

const int WIDTH = 50;
const int HEIGHT = 25;
const string HIGH_SCORE_FILE = "score.txt";

// ------------------ Cursor Movement for Smooth Screen ------------------
void setCursorPosition(int x, int y) {
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(output, pos);
}

// =========================== Class: Snake ===========================
class Snake {
private:
    vector<pair<int, int>> body;
    int direction;

public:
    enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

    Snake() {
        reset();
    }

    void reset() {
        body.clear();
        body.push_back({WIDTH / 2, HEIGHT / 2});
        body.push_back({WIDTH / 2 - 1, HEIGHT / 2});
        body.push_back({WIDTH / 2 - 2, HEIGHT / 2});
        direction = STOP;
    }

    pair<int, int> getHead() const { return body[0]; }
    vector<pair<int, int>> getBody() const { return body; }

    void setDirection(int dir) { direction = dir; }
    int getDirection() const { return direction; }

    void move(bool grow = false) {
        if (direction == STOP) return;

        pair<int, int> newHead = body[0];
        switch (direction) {
            case LEFT: newHead.first--; break;
            case RIGHT: newHead.first++; break;
            case UP: newHead.second--; break;
            case DOWN: newHead.second++; break;
        }
        body.insert(body.begin(), newHead);
        if (!grow) body.pop_back();
    }

    bool isBody(int x, int y) const {
        for (size_t i = 1; i < body.size(); ++i)
            if (body[i].first == x && body[i].second == y)
                return true;
        return false;
    }

    bool checkCollision() const {
        pair<int, int> head = body[0];
        for (size_t i = 1; i < body.size(); ++i)
            if (body[i] == head)
                return true;
        return false;
    }

    int length() const { return body.size(); }
};

// =========================== Class: Food ===========================
class Food {
private:
    pair<int, int> position;
    string symbol;
    int points;

    vector<pair<string, int>> fruits = {
        {"🍎", 10}, {"🍇", 15}, {"🍓", 20},
        {"🍊", 25}, {"🍌", 30}, {"🍍", 40}
    };

public:
    void generate(const vector<pair<int, int>>& snakeBody, const vector<pair<int, int>>& obstacles) {
        bool conflict;
        do {
            conflict = false;
            position.first = rand() % (WIDTH - 4) + 2;
            position.second = rand() % (HEIGHT - 4) + 2;

            // check not on snake
            for (auto s : snakeBody)
                if (s == position) { conflict = true; break; }

            // check not on obstacle
            for (auto o : obstacles)
                if (o == position) { conflict = true; break; }

        } while (conflict);

        int fruitIndex = rand() % fruits.size();
        symbol = fruits[fruitIndex].first;
        points = fruits[fruitIndex].second;
    }

    pair<int, int> getPosition() const { return position; }
    string getSymbol() const { return symbol; }
    int getPoints() const { return points; }
};

// =========================== Class: Obstacle ===========================

class Obstacle {
private:
    vector<pair<int, int>> blocks;
    const int count = 5; // ✅ only 4–5 obstacles for balanced difficulty

public:
    void generate(const vector<pair<int, int>>& snakeBody, const pair<int, int>& foodPos) {
        blocks.clear();
        bool conflict;
        for (int i = 0; i < count; i++) {
            pair<int, int> pos;
            do {
                conflict = false;
                pos.first = rand() % (WIDTH - 4) + 2;
                pos.second = rand() % (HEIGHT - 4) + 2;

                // Prevent spawning on snake, food, or existing obstacles
                for (auto s : snakeBody)
                    if (s == pos) { conflict = true; break; }
                if (pos == foodPos) conflict = true;
                for (auto b : blocks)
                    if (b == pos) { conflict = true; break; }

            } while (conflict);
            blocks.push_back(pos);
        }
    }

    bool isObstacle(int x, int y) const {
        for (auto b : blocks)
            if (b.first == x && b.second == y)
                return true;
        return false;
    }

    const vector<pair<int, int>>& getBlocks() const { return blocks; }
};


// =========================== Class: SnakeGame ===========================
class SnakeGame {
private:
    Snake snake;
    Food food;
    Obstacle obstacle;
    int score;
    int speed;
    int fruitsEaten;
    int highScore;
    bool gameOver;
    string screenBuffer;

public:
    SnakeGame() {
        highScore = loadHighScore();
        resetGame();
    }

    int loadHighScore() {
        ifstream inFile(HIGH_SCORE_FILE);
        int hs = 0;
        if (inFile.is_open()) {
            inFile >> hs;
            inFile.close();
        }
        return hs;
    }

    void saveHighScore() {
        if (score > highScore) {
            ofstream outFile(HIGH_SCORE_FILE);
            if (outFile.is_open()) {
                outFile << score;
                outFile.close();
                highScore = score;
            }
        }
    }

    void resetGame() {
        snake.reset();
        obstacle.generate(snake.getBody(), { -1, -1 });
        food.generate(snake.getBody(), obstacle.getBlocks());
        score = 0;
        speed = 130;
        fruitsEaten = 0;
        gameOver = false;
        screenBuffer.reserve((WIDTH + 3) * (HEIGHT + 10));
    }

    void drawHeader() {
        screenBuffer += "  +==================================================+\n";
        screenBuffer += "  |             SNAKE GAME v4.0 (With Obstacles)     |\n";
        screenBuffer += "  +==================================================+\n";
    }

    void drawGame() {
        screenBuffer += "  🧱";
        for (int i = 0; i < WIDTH; i++) screenBuffer += "🧱";
        screenBuffer += "🧱\n";

        auto snakeBody = snake.getBody();
        for (int y = 0; y < HEIGHT; y++) {
            screenBuffer += "  🧱";
            for (int x = 0; x < WIDTH; x++) {
                if (x == snakeBody[0].first && y == snakeBody[0].second)
                    screenBuffer += "😎";
                else if (snake.isBody(x, y))
                    screenBuffer += "🟢";
                else if (obstacle.isObstacle(x, y))
                    screenBuffer += "💀"; // obstacle symbol
                else if (x == food.getPosition().first && y == food.getPosition().second)
                    screenBuffer += food.getSymbol();
                else
                    screenBuffer += "  ";
            }
            screenBuffer += "🧱\n";
        }

        screenBuffer += "  🧱";
        for (int i = 0; i < WIDTH; i++) screenBuffer += "🧱";
        screenBuffer += "🧱\n";
    }

    void drawStats() {
        screenBuffer += "\n";
        screenBuffer += "  STATISTICS:\n";
        screenBuffer += "  ==================================================\n";
        screenBuffer += "  Score: " + to_string(score) + " points\n";
        screenBuffer += "  High Score: " + to_string(highScore) + " points\n";
        screenBuffer += "  Fruits Eaten: " + to_string(fruitsEaten) + "\n";
        screenBuffer += "  Snake Length: " + to_string(snake.length()) + "\n";
    }

    void draw() {
        screenBuffer.clear();
        drawHeader();
        screenBuffer += "\n";
        drawGame();
        drawStats();
        setCursorPosition(0, 0);
        cout << screenBuffer;
    }

    void input() {
        if (_kbhit()) {
            int key = _getch();
            if (key == 224 || key == 0) {
                key = _getch();
                switch (key) {
                    case 75: if (snake.getDirection() != Snake::RIGHT) snake.setDirection(Snake::LEFT); break;
                    case 77: if (snake.getDirection() != Snake::LEFT) snake.setDirection(Snake::RIGHT); break;
                    case 72: if (snake.getDirection() != Snake::DOWN) snake.setDirection(Snake::UP); break;
                    case 80: if (snake.getDirection() != Snake::UP) snake.setDirection(Snake::DOWN); break;
                }
            } else {
                switch (key) {
                    case 'a': case 'A': if (snake.getDirection() != Snake::RIGHT) snake.setDirection(Snake::LEFT); break;
                    case 'd': case 'D': if (snake.getDirection() != Snake::LEFT) snake.setDirection(Snake::RIGHT); break;
                    case 'w': case 'W': if (snake.getDirection() != Snake::DOWN) snake.setDirection(Snake::UP); break;
                    case 's': case 'S': if (snake.getDirection() != Snake::UP) snake.setDirection(Snake::DOWN); break;
                    case 'p': case 'P': pauseGame(); break;
                    case 'x': case 'X': gameOver = true; break;
                }
            }
        }
    }

    void pauseGame() {
        cout << "\n  GAME PAUSED. Press any key to continue...\n";
        _getch();
    }

    void logic() {
        snake.move();

        auto head = snake.getHead();
        if (head.first < 0 || head.first >= WIDTH || head.second < 0 || head.second >= HEIGHT) {
            gameOver = true;
            return;
        }

        if (snake.checkCollision()) {
            gameOver = true;
            return;
        }

        if (obstacle.isObstacle(head.first, head.second)) {
            gameOver = true;
            return;
        }

        if (head == food.getPosition()) {
            score += food.getPoints();
            fruitsEaten++;
            snake.move(true);
            obstacle.generate(snake.getBody(), food.getPosition());
            food.generate(snake.getBody(), obstacle.getBlocks());
            if (fruitsEaten % 5 == 0 && speed > 50) speed -= 5;
        }
    }

    void showGameOver() {
        system("cls");
        cout << "  +==================================================+\n";
        cout << "  |                   GAME OVER!                     |\n";
        cout << "  +==================================================+\n\n";
        cout << "  Final Score: " << score << " points\n";
        cout << "  High Score: " << (score > highScore ? score : highScore) << " points\n";
        cout << "  Fruits Eaten: " << fruitsEaten << "\n";
        cout << "  Snake Length: " << snake.length() << "\n\n";
        saveHighScore();
        cout << "  Press R to Restart or X to Exit...\n";
    }

    void run() {
        cout << "  +==================================================+\n";
        cout << "  |           WELCOME TO SNAKE GAME!                 |\n";
        cout << "  |           Press any key to start!                |\n";
        cout << "  +==================================================+\n";
        _getch();

        while (true) {
            while (!gameOver) {
                draw();
                input();
                logic();
                Sleep(speed);
            }

            showGameOver();
            char ch = _getch();
            if (ch == 'r' || ch == 'R') resetGame();
            else break;
        }
    }
};

// =========================== Console Setup ===========================
void setupConsole() {
    system("chcp 65001 > nul");
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
    SetConsoleTitleA("Snake Game - With Obstacles");
    system("mode con: cols=70 lines=45");
}

int main() {
    srand((unsigned)time(NULL)); // seed random once
    setupConsole();
    SnakeGame game;
    game.run();
    return 0;
}
