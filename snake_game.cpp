#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>

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
    
    enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

public:
    SnakeGame() {
        gameOver = false;
        direction = STOP;
        score = 0;
        speed = 100;
        fruitsEaten = 0;
        
        // Initialize snake with 3 segments
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

    void draw() {
        system("cls");
        
        // Draw borders and game area
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;
        
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (x == 0) cout << "#";
                
                if (x == snake[0].first && y == snake[0].second)
                    cout << "O"; // Head
                else if (isSnakeBody(x, y))
                    cout << "o"; // Body
                else if (x == food.first && y == food.second)
                    cout << "F";
                else
                    cout << " ";
                
                if (x == WIDTH - 1) cout << "#";
            }
            cout << endl;
        }
        
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;
        
        cout << "Score: " << score << " | Fruits: " << fruitsEaten << " | Length: " << snake.size() << endl;
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
            switch (_getch()) {
                case 'a': if (direction != RIGHT) direction = LEFT; break;
                case 'd': if (direction != LEFT) direction = RIGHT; break;
                case 'w': if (direction != DOWN) direction = UP; break;
                case 's': if (direction != UP) direction = DOWN; break;
                case 'x': gameOver = true; break;
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
        
        // Wall collision
        if (newHead.first < 0 || newHead.first >= WIDTH || 
            newHead.second < 0 || newHead.second >= HEIGHT) {
            gameOver = true;
            return;
        }
        
        // Self collision
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
            
            // Increase speed
            if (fruitsEaten % 5 == 0 && speed > 50) {
                speed -= 10;
            }
            
            generateFood();
        } else {
            snake.pop_back();
        }
    }

    void run() {
        while (!gameOver) {
            draw();
            input();
            logic();
            Sleep(speed);
        }
        
        cout << "Game Over! Final Score: " << score << endl;
    }
};

int main() {
    SnakeGame game;
    game.run();
    return 0;
}
