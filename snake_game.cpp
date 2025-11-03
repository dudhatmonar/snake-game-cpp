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
    
    enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

public:
    SnakeGame() {
        gameOver = false;
        direction = STOP;
        score = 0;
        
        // Initialize snake in the middle
        snake.push_back({WIDTH / 2, HEIGHT / 2});
        
        // Generate first food
        generateFood();
    }

    void generateFood() {
        srand(time(0));
        food.first = rand() % WIDTH;
        food.second = rand() % HEIGHT;
    }

    void draw() {
        system("cls");
        
        // Draw top border
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;
        
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                // Draw left border
                if (x == 0) cout << "#";
                
                // Draw snake head
                if (x == snake[0].first && y == snake[0].second)
                    cout << "O";
                // Draw food
                else if (x == food.first && y == food.second)
                    cout << "F";
                // Draw empty space
                else
                    cout << " ";
                
                // Draw right border
                if (x == WIDTH - 1) cout << "#";
            }
            cout << endl;
        }
        
        // Draw bottom border
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;
        
        cout << "Score: " << score << endl;
    }

    void input() {
        if (_kbhit()) {
            switch (_getch()) {
                case 'a': direction = LEFT; break;
                case 'd': direction = RIGHT; break;
                case 'w': direction = UP; break;
                case 's': direction = DOWN; break;
                case 'x': gameOver = true; break;
            }
        }
    }

    void logic() {
        if (direction == STOP) return;
        
        // Move head based on direction
        pair<int, int> newHead = snake[0];
        switch (direction) {
            case LEFT: newHead.first--; break;
            case RIGHT: newHead.first++; break;
            case UP: newHead.second--; break;
            case DOWN: newHead.second++; break;
        }
        
        // Check wall collision
        if (newHead.first < 0 || newHead.first >= WIDTH || 
            newHead.second < 0 || newHead.second >= HEIGHT) {
            gameOver = true;
            return;
        }
        
        // Move snake
        snake.insert(snake.begin(), newHead);
        
        // Check food collision
        if (newHead.first == food.first && newHead.second == food.second) {
            score += 10;
            generateFood();
        } else {
            // Remove tail if no food eaten
            if (snake.size() > 1) {
                snake.pop_back();
            }
        }
    }

    void run() {
        while (!gameOver) {
            draw();
            input();
            logic();
            Sleep(100);
        }
        
        cout << "Game Over! Final Score: " << score << endl;
    }
};

int main() {
    SnakeGame game;
    game.run();
    return 0;
}