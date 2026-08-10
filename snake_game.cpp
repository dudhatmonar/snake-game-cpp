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

    Snake(int startX = WIDTH / 2, int startY = HEIGHT / 2) {
        reset(startX, startY);
    }

    void reset(int startX = WIDTH / 2, int startY = HEIGHT / 2) {
        body.clear();
        body.push_back({startX, startY});
        body.push_back({startX - 1, startY});
        body.push_back({startX - 2, startY});
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

    bool checkCollisionWithOther(const Snake &other) const {
        pair<int, int> head = body[0];
        auto otherBody = other.getBody();
        for (size_t i = 0; i < otherBody.size(); ++i)
            if (otherBody[i] == head)
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
    void generate(const vector<pair<int, int>>& snake1Body, const vector<pair<int, int>>& snake2Body, const vector<pair<int, int>>& obstacles) {
        bool conflict;
        do {
            conflict = false;
            position.first = rand() % (WIDTH - 4) + 2;
            position.second = rand() % (HEIGHT - 4) + 2;

            // check not on snake 1
            for (auto s : snake1Body)
                if (s == position) { conflict = true; break; }

            // check not on snake 2
            for (auto s : snake2Body)
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
    void generate(const vector<pair<int, int>>& snake1Body, const vector<pair<int, int>>& snake2Body, const pair<int, int>& foodPos) {
        blocks.clear();
        bool conflict;
        for (int i = 0; i < count; i++) {
            pair<int, int> pos;
            do {
                conflict = false;
                pos.first = rand() % (WIDTH - 4) + 2;
                pos.second = rand() % (HEIGHT - 4) + 2;

                // Prevent spawning on snakes, food, or existing obstacles
                for (auto s : snake1Body)
                    if (s == pos) { conflict = true; break; }
                for (auto s : snake2Body)
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
    Snake snake1;
    Snake snake2;
    Food food;
    Obstacle obstacle;
    int score1;
    int score2;
    int speed;
    int fruitsEaten1;
    int fruitsEaten2;
    int highScore;
    bool gameOver;
    int loser;
    string screenBuffer;

public:
    SnakeGame() : snake1(WIDTH / 3, HEIGHT / 2), snake2(2 * WIDTH / 3, HEIGHT / 2) {
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
        int topScore = max(score1, score2);
        if (topScore > highScore) {
            ofstream outFile(HIGH_SCORE_FILE);
            if (outFile.is_open()) {
                outFile << topScore;
                outFile.close();
                highScore = topScore;
            }
        }
    }

    void resetGame() {
        snake1.reset(WIDTH / 3, HEIGHT / 2);
        snake2.reset(2 * WIDTH / 3, HEIGHT / 2);
        snake1.setDirection(Snake::RIGHT);
        snake2.setDirection(Snake::LEFT);
        obstacle.generate(snake1.getBody(), snake2.getBody(), { -1, -1 });
        food.generate(snake1.getBody(), snake2.getBody(), obstacle.getBlocks());
        score1 = 0;
        score2 = 0;
        speed = 130;
        fruitsEaten1 = 0;
        fruitsEaten2 = 0;
        gameOver = false;
        loser = 0;
        screenBuffer.reserve((WIDTH + 3) * (HEIGHT + 10));
    }

    void drawHeader() {
        screenBuffer += "  +==================================================+\n";
        screenBuffer += "  |             SNAKE GAME v4.0                      |\n";
        screenBuffer += "  +==================================================+\n";
    }

    void drawGame() {
        screenBuffer += "  🧱";
        for (int i = 0; i < WIDTH; i++) screenBuffer += "🧱";
        screenBuffer += "🧱\n";

        auto body1 = snake1.getBody();
        auto body2 = snake2.getBody();
        for (int y = 0; y < HEIGHT; y++) {
            screenBuffer += "  🧱";
            for (int x = 0; x < WIDTH; x++) {
                if ((x == body1[0].first && y == body1[0].second) ||
                    (x == body2[0].first && y == body2[0].second))
                    screenBuffer += "😎";
                else if (snake1.isBody(x, y) || snake2.isBody(x, y))
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
        screenBuffer += "  P1 Score: " + to_string(score1) + " | P2 Score: " + to_string(score2) + "\n";
        screenBuffer += "  High Score: " + to_string(highScore) + " points\n";
        screenBuffer += "  P1 Fruits: " + to_string(fruitsEaten1) + " | P2 Fruits: " + to_string(fruitsEaten2) + "\n";
        screenBuffer += "  P1 Length: " + to_string(snake1.length()) + " | P2 Length: " + to_string(snake2.length()) + "\n";
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
                    case 75: if (snake1.getDirection() != Snake::RIGHT) snake1.setDirection(Snake::LEFT); break;
                    case 77: if (snake1.getDirection() != Snake::LEFT) snake1.setDirection(Snake::RIGHT); break;
                    case 72: if (snake1.getDirection() != Snake::DOWN) snake1.setDirection(Snake::UP); break;
                    case 80: if (snake1.getDirection() != Snake::UP) snake1.setDirection(Snake::DOWN); break;
                }
            } else {
                switch (key) {
                    case 'a': case 'A': if (snake2.getDirection() != Snake::RIGHT) snake2.setDirection(Snake::LEFT); break;
                    case 'd': case 'D': if (snake2.getDirection() != Snake::LEFT) snake2.setDirection(Snake::RIGHT); break;
                    case 'w': case 'W': if (snake2.getDirection() != Snake::DOWN) snake2.setDirection(Snake::UP); break;
                    case 's': case 'S': if (snake2.getDirection() != Snake::UP) snake2.setDirection(Snake::DOWN); break;
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
        snake1.move();
        snake2.move();

        auto head1 = snake1.getHead();
        auto head2 = snake2.getHead();

        bool p1Lost = (head1.first < 0 || head1.first >= WIDTH || head1.second < 0 || head1.second >= HEIGHT) ||
                      snake1.checkCollision() || snake1.checkCollisionWithOther(snake2) ||
                      obstacle.isObstacle(head1.first, head1.second);

        bool p2Lost = (head2.first < 0 || head2.first >= WIDTH || head2.second < 0 || head2.second >= HEIGHT) ||
                      snake2.checkCollision() || snake2.checkCollisionWithOther(snake1) ||
                      obstacle.isObstacle(head2.first, head2.second);

        if (head1 == head2 && snake1.getDirection() != Snake::STOP && snake2.getDirection() != Snake::STOP) {
            p1Lost = true;
            p2Lost = true;
        }

        if (p1Lost || p2Lost) {
            gameOver = true;
            if (p1Lost && p2Lost) loser = 3;
            else if (p1Lost) loser = 1;
            else if (p2Lost) loser = 2;
            return;
        }

        if (head1 == food.getPosition()) {
            score1 += food.getPoints();
            fruitsEaten1++;
            snake1.move(true);
            obstacle.generate(snake1.getBody(), snake2.getBody(), food.getPosition());
            food.generate(snake1.getBody(), snake2.getBody(), obstacle.getBlocks());
            if ((fruitsEaten1 + fruitsEaten2) % 5 == 0 && speed > 50) speed -= 5;
        }

        if (head2 == food.getPosition()) {
            score2 += food.getPoints();
            fruitsEaten2++;
            snake2.move(true);
            obstacle.generate(snake1.getBody(), snake2.getBody(), food.getPosition());
            food.generate(snake1.getBody(), snake2.getBody(), obstacle.getBlocks());
            if ((fruitsEaten1 + fruitsEaten2) % 5 == 0 && speed > 50) speed -= 5;
        }
    }

    void showGameOver() {
        system("cls");
        cout << "  +==================================================+\n";
        cout << "  |                   GAME OVER!                     |\n";
        cout << "  +==================================================+\n\n";
        if (loser == 1)
            cout << "  🔴 Player 1 (Arrow Keys) Lost!\n\n";
        else if (loser == 2)
            cout << "  🔵 Player 2 (WASD) Lost!\n\n";
        else if (loser == 3)
            cout << "  💥 Both Players Lost!\n\n";

        cout << "  P1 Final Score: " << score1 << " points | Fruits: " << fruitsEaten1 << "\n";
        cout << "  P2 Final Score: " << score2 << " points | Fruits: " << fruitsEaten2 << "\n\n";
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
