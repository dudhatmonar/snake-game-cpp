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
    
    // Different fruit types with their symbols and points - using string for emojis
    vector<pair<string, int>> fruits = {
        {"A", 10},  // Apple (fallback - will try emoji later)
        {"G", 15},  // Grapes (fallback)
        {"S", 20},  // Strawberry (fallback)
        {"O", 25},  // Orange (fallback)
        {"B", 30},  // Banana (fallback)
        {"P", 40}   // Pineapple (fallback)
    };
    
    string currentFruit;
    int currentFruitPoints;
    bool emojiSupported;

    enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

public:
    SnakeGame() {
        gameOver = false;
        direction = STOP;
        score = 0;
        speed = 100;
        fruitsEaten = 0;
        emojiSupported = false;
        screenBuffer.reserve((WIDTH + 3) * (HEIGHT + 10));
        
        // Try to initialize with emojis
        initializeFruitsWithEmojis();
        
        // Initialize snake in the middle with 3 segments
        snake.push_back({WIDTH / 2, HEIGHT / 2});
        snake.push_back({WIDTH / 2 - 1, HEIGHT / 2});
        snake.push_back({WIDTH / 2 - 2, HEIGHT / 2});
        
        // Generate first food
        generateFood();
    }

    void initializeFruitsWithEmojis() {
        // Try to use emojis - if they don't display, fallback to letters
        vector<pair<string, int>> emojiFruits = {
            {"🍎", 10},  // Apple 🍎
            {"🍇", 15},  // Grapes 🍇
            {"\U0001F353", 20},  // Strawberry 🍓
            {"\U0001F34A", 25},  // Orange 🍊
            {"\U0001F34C", 30},  // Banana 🍌
            {"\U0001F34D", 40}   // Pineapple 🍍
        };
        
        // Test if emojis work by printing one
        cout << "Testing emoji: " << emojiFruits[0].first << endl;
        cout << "If you see an apple emoji above, emojis are supported!" << endl;
        cout << "If you see strange characters, emojis are not supported." << endl;
        cout << "Press any key to continue..." << endl;
        _getch();
        system("cls");
        
        // For now, use fallback letters to ensure the game works
        // You can change this to emojiFruits if emojis work on your system
        fruits = {
            {"\U0001F34E", 10},  // Apple 🍎
            {"\U0001F347", 15},  // Grapes 🍇
            {"\U0001F353", 20},  // Strawberry 🍓
            {"\U0001F34A", 25},  // Orange 🍊
            {"\U0001F34C", 30},  // Banana 🍌
            {"\U0001F34D", 40}   // Pineapple 🍍
        };
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
        
        // Select random fruit
        int fruitIndex = rand() % fruits.size();
        currentFruit = fruits[fruitIndex].first;
        currentFruitPoints = fruits[fruitIndex].second;
    }

    // void clearScreen() {
    //     system("cls");
    // }

    void drawHeader() {
        screenBuffer += "  +==================================================+\n";
        screenBuffer += "  |               SNAKE GAME v2.0                    |\n";
        screenBuffer += "  +==================================================+\n";
    }

    void drawGameBorder() {
        // Draw top border with corners
        screenBuffer += "  🧱";
        for (int i = 0; i < WIDTH; i++) screenBuffer += "🧱";
        screenBuffer += "🧱\n";
        
        // Draw game area with side borders
        for (int y = 0; y < HEIGHT; y++) {
            screenBuffer += "  🧱";
            for (int x = 0; x < WIDTH; x++) {
                // Draw snake head
                if (x == snake[0].first && y == snake[0].second) {
                    screenBuffer += "😎";  // Cool snake head
                }
                // Draw snake body with gradient colors
                else if (isSnakeBody(x, y)) {
                    int segmentIndex = getSnakeSegmentIndex(x, y);
                    if (segmentIndex < snake.size() / 3) {
                        screenBuffer += "🟢";  // Green segments near head
                    } else if (segmentIndex < 2 * snake.size() / 3) {
                        screenBuffer += "🟡";  // Yellow middle segments
                    } else {
                        screenBuffer += "🟠";  // Orange segments near tail
                    }
                }
                // Draw food
                else if (x == food.first && y == food.second) {
                    screenBuffer += currentFruit;
                }
                // Draw empty space
                else {
                    screenBuffer += "  ";  // Two spaces for emoji alignment
                }
            }
            screenBuffer += "🧱\n";
        }
        
        // Draw bottom border
        screenBuffer += "  🧱";
        for (int i = 0; i < WIDTH; i++) screenBuffer += "🧱";
        screenBuffer += "🧱\n";
    }

    int getSnakeSegmentIndex(int x, int y) {
        for (int i = 1; i < snake.size(); i++) {
            if (snake[i].first == x && snake[i].second == y)
                return i;
        }
        return -1;
    }

    void drawStats() {
        screenBuffer += "\n";
        screenBuffer += "  STATISTICS:\n";
        screenBuffer += "  ==================================================\n";
        screenBuffer += "  Score: " + to_string(score) + " points\n";
        screenBuffer += "  Fruits Eaten: " + to_string(fruitsEaten) + "\n";
        screenBuffer += "  Snake Length: " + to_string(snake.size()) + "\n";
        
        // Show fruit name based on symbol
        string fruitName = getFruitName(currentFruit);
        screenBuffer += "  Current Fruit: " + fruitName + " " + currentFruit + " (" + to_string(currentFruitPoints) + " points)\n";
    }

    string getFruitName(const string& fruitSymbol) {
        if (fruitSymbol == "A") return "Apple";
        if (fruitSymbol == "G") return "Grapes";
        if (fruitSymbol == "S") return "Strawberry";
        if (fruitSymbol == "O") return "Orange";
        if (fruitSymbol == "B") return "Banana";
        if (fruitSymbol == "P") return "Pineapple";
        
        // For emojis
        if (fruitSymbol == "🍎") return "Apple";
        if (fruitSymbol == "🍇") return "Grapes";
        if (fruitSymbol == "\U0001F353") return "Strawberry";
        if (fruitSymbol == "\U0001F34A") return "Orange";
        if (fruitSymbol == "\U0001F34C") return "Banana";
        if (fruitSymbol == "\U0001F34D") return "Pineapple";
        
        return "Fruit";
    }

    void drawControls() {
        screenBuffer += "\n";
        screenBuffer += "  CONTROLS:\n";
        screenBuffer += "  ==================================================\n";
        screenBuffer += "   W / Arrow Keys   Move Snake\n";
        screenBuffer += "   X                Exit Game\n";
        screenBuffer += "   P                Pause Game\n";
    }

    void draw() {
        screenBuffer.clear();
        
        // Draw all components
        drawHeader();
        screenBuffer += "\n";
        drawGameBorder();
        drawStats();
        // drawControls();
        
        // Print entire buffer at once
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
            
            // Handle arrow keys
            if (key == 224 || key == 0) {
                key = _getch();
                switch (key) {
                    case 75: // Left arrow
                        if (direction != RIGHT) direction = LEFT;
                        break;
                    case 77: // Right arrow
                        if (direction != LEFT) direction = RIGHT;
                        break;
                    case 72: // Up arrow
                        if (direction != DOWN) direction = UP;
                        break;
                    case 80: // Down arrow
                        if (direction != UP) direction = DOWN;
                        break;
                }
            }
            // Handle regular keys
            else {
                switch (key) {
                    case 'a': case 'A':
                        if (direction != RIGHT) direction = LEFT;
                        break;
                    case 'd': case 'D':
                        if (direction != LEFT) direction = RIGHT;
                        break;
                    case 'w': case 'W':
                        if (direction != DOWN) direction = UP;
                        break;
                    case 's': case 'S':
                        if (direction != UP) direction = DOWN;
                        break;
                    case 'x': case 'X':
                        gameOver = true;
                        break;
                    case 'p': case 'P':
                        pauseGame();
                        break;
                }
            }
        }
    }

    void pauseGame() {
        // clearScreen();
        cout << "  +==================================================+\n";
        cout << "  |               GAME PAUSED                        |\n";
        cout << "  |                                                  |\n";
        cout << "  |           Press any key to continue              |\n";
        cout << "  +==================================================+\n";
        _getch();
    }

    void logic() {
        if (direction == STOP) return;
        
        // Store current head position
        pair<int, int> newHead = snake[0];
        
        // Move head based on direction
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
        
        // Check self collision
        for (auto segment : snake) {
            if (segment.first == newHead.first && segment.second == newHead.second) {
                gameOver = true;
                return;
            }
        }
        
        // Move snake
        snake.insert(snake.begin(), newHead);
        
        // Check food collision
        if (newHead.first == food.first && newHead.second == food.second) {
            score += currentFruitPoints;
            fruitsEaten++;
            
            // Increase speed slightly every 5 fruits
            if (fruitsEaten % 5 == 0 && speed > 50) {
                speed -= 5;
            }
            
            generateFood();
        } else {
            // Remove tail if no food eaten
            snake.pop_back();
        }
    }

    void showGameOver() {
        // clearScreen();
        cout << "  +==================================================+\n";
        cout << "  |                   GAME OVER!                     |\n";
        cout << "  +==================================================+\n\n";
        
        cout << "  Final Statistics:\n";
        cout << "  ==================================================\n";
        cout << "  Score: " << score << " points\n";
        cout << "  Fruits Eaten: " << fruitsEaten << "\n";
        cout << "  Snake Length: " << snake.size() << "\n";
        cout << "  Highest Fruit: " << getFruitName(currentFruit) << " (" << currentFruitPoints << " points)\n\n";
        
        cout << "  Press any key to exit...\n";
    }

    void run() {
        // clearScreen();
        
        // Show welcome screen
        cout << "  +==================================================+\n";
        cout << "  |           WELCOME TO SNAKE GAME!                 |\n";
        cout << "  |                                                  |\n";
        cout << "  |           Get ready to play...                   |\n";
        cout << "  |                                                  |\n";
        cout << "  |           Press any key to start!                |\n";
        cout << "  +==================================================+\n";
        _getch();
        
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

// Function to set console properties
void setupConsole() {
    // Set UTF-8 encoding for emoji support
    system("chcp 65001 > nul");
    
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Hide cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
    
    // Set console title
    SetConsoleTitleA("Snake Game - Enhanced Version");
    
    // Set console size
    system("mode con: cols=70 lines=45");
}

int main() {
    setupConsole();
    SnakeGame game;
    game.run();
    return 0;
}
