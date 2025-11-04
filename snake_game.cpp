#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

const int WIDTH = 50;
const int HEIGHT = 25;

class SnakeGame {
private:
    vector<pair<int, int>> snake;
    pair<int, int> food;
    int direction;
    bool gameOver;
    int score;
    int speed;
    int fruitsEaten;
    string screenBuffer;
    
    enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

public:
    SnakeGame() {
        gameOver = false;
        direction = STOP;
        score = 0;
        speed = 100;
        fruitsEaten = 0;
        screenBuffer.reserve((WIDTH + 3) * (HEIGHT + 6));
        
        snake.push_back({WIDTH / 2, HEIGHT / 2});
        snake.push_back({WIDTH / 2 - 1, HEIGHT / 2});
        snake.push_back({WIDTH / 2 - 2, HEIGHT / 2});
        
        generateFood();
    }

    void generateFood() {
        srand(time(0));
        bool onSnake;
        do {
            onSnake = false;
            food.first = rand() % (WIDTH - 4) + 2;
            food.second = rand() % (HEIGHT - 4) + 2;
            
            for (auto segment : snake) {
                if (segment.first == food.first && segment.second == food.second) {
                    onSnake = true;
                    break;
                }
            }
        } while (onSnake);
    }

    void drawHeader() {
        screenBuffer += "  +==================================================+\n";
        screenBuffer += "  |               SNAKE GAME v1.0                  |\n";
        screenBuffer += "  +==================================================+\n";
    }

    void drawGameBorder() {
        screenBuffer += "  +";
        for (int i = 0; i < WIDTH; i++) screenBuffer += "=";
        screenBuffer += "+\n";
        
        for (int y = 0; y < HEIGHT; y++) {
            screenBuffer += "  |";
            for (int x = 0; x < WIDTH; x++) {
                if (x == snake[0].first && y == snake[0].second)
                    screenBuffer += "O";
                else if (isSnakeBody(x, y))
                    screenBuffer += "o";
                else if (x == food.first && y == food.second)
                    screenBuffer += "F";
                else
                    screenBuffer += " ";
            }
            screenBuffer += "|\n";
        }
        
        screenBuffer += "  +";
        for (int i = 0; i < WIDTH; i++) screenBuffer += "=";
        screenBuffer += "+\n";
    }

    void drawStats() {
        screenBuffer += "\n";
        screenBuffer += "  STATISTICS:\n";
        screenBuffer += "  ==================================================\n";
        screenBuffer += "  Score: " + to_string(score) + " points\n";
        screenBuffer += "  Fruits Eaten: " + to_string(fruitsEaten) + "\n";
        screenBuffer += "  Snake Length: " + to_string(snake.size()) + "\n";
    }

    void drawControls() {
        screenBuffer += "\n";
        screenBuffer += "  CONTROLS: WASD to move, X to exit\n";
    }

    void draw() {
        screenBuffer.clear();
        
        drawHeader();
        screenBuffer += "\n";
        drawGameBorder();
        drawStats();
        drawControls();
        
        cout << screenBuffer;
    }

    bool isSnakeBody(int x, int y) {
        for (int i = 1; i < snake.size(); i++) {
            if (snake[i].first == x && snake[i].second == y)
                return true;
        }
        return false;
    }

    void input() {
        if (_kbhit()) {
            int key = _getch();
            
            if (key == 224 || key == 0) {
                key = _getch();
                switch (key) {
                    case 75: if (direction != RIGHT) direction = LEFT; break;
                    case 77: if (direction != LEFT) direction = RIGHT; break;
                    case 72: if (direction != DOWN) direction = UP; break;
                    case 80: if (direction != UP) direction = DOWN; break;
                }
            } else {
                switch (key) {
                    case 'a': case 'A': if (direction != RIGHT) direction = LEFT; break;
                    case 'd': case 'D': if (direction != LEFT) direction = RIGHT; break;
                    case 'w': case 'W': if (direction != DOWN) direction = UP; break;
                    case 's': case 'S': if (direction != UP) direction = DOWN; break;
                    case 'x': case 'X': gameOver = true; break;
                }
            }
        }
    }

    void logic() {
        if (direction == STOP) return;
        
        pair<int, int> newHead = snake[0];
        switch (direction) {
            case LEFT: newHead.first--; break;
            case RIGHT: newHead.first++; break;
            case UP: newHead.second--; break;
            case DOWN: newHead.second++; break;
        }
        
        if (newHead.first < 0 || newHead.first >= WIDTH || 
            newHead.second < 0 || newHead.second >= HEIGHT) {
            gameOver = true;
            return;
        }
        
        for (auto segment : snake) {
            if (segment.first == newHead.first && segment.second == newHead.second) {
                gameOver = true;
                return;
            }
        }
        
        snake.insert(snake.begin(), newHead);
        
        if (newHead.first == food.first && newHead.second == food.second) {
            score += 10;
            fruitsEaten++;
            
            if (fruitsEaten % 5 == 0 && speed > 50) {
                speed -= 5;
            }
            
            generateFood();
        } else {
            snake.pop_back();
        }
    }

    void showGameOver() {
        system("cls");
        cout << "  +==================================================+\n";
        cout << "  |                   GAME OVER!                    |\n";
        cout << "  +==================================================+\n\n";
        cout << "  Final Score: " << score << " points\n";
        cout << "  Fruits Eaten: " << fruitsEaten << "\n";
        cout << "  Snake Length: " << snake.size() << "\n\n";
        cout << "  Press any key to exit...\n";
    }

    void run() {
        while (!gameOver) {
            draw();
            input();
            logic();
            Sleep(speed);
        }
        
        showGameOver();
        _getch();
    }
};

int main() {
    // Setup console
    system("mode con: cols=60 lines=40");
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
    
    SnakeGame game;
    game.run();
    return 0;
}
