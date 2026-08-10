# LLM Log

## 1. Setup

| Field | Value |
|---|---|
| **Model(s)** | Gemini 3.6 Flash (High) |
| **Tool / harness** | Antigravity AI Coding Assistant |
| **IDE / editor** | VS Code / Antigravity IDE |
| **Did you paste this assignment document into it?** | y — whole assignment document (Lab Assignment 3) |

---

## 2. Session Log

### Prompt 1
> "Clone the repository https://github.com/dudhatmonar/snake-game-cpp, create the branch feat/multiplayer, and implement 2-player multiplayer support according to the requirements in Lab Assignment 3."

**Assistant Reply Summary**:
Cloned target repository `dudhatmonar/snake-game-cpp` into local directory, created branch `feat/multiplayer`, set `core.autocrlf false`, analyzed `snake_game.cpp`, implemented 2-player support, and compiled `snake_game.exe`.

```cpp
// Key code snippet added to snake_game.cpp for 2-player support:
class SnakeGame {
private:
    Snake snake1;
    Snake snake2;
    Food food;
    Obstacle obstacle;
    int score1, score2;
    int fruitsEaten1, fruitsEaten2;
    int loser;

public:
    SnakeGame() : snake1(WIDTH / 3, HEIGHT / 2), snake2(2 * WIDTH / 3, HEIGHT / 2) {
        highScore = loadHighScore();
        resetGame();
    }

    void input() {
        if (_kbhit()) {
            int key = _getch();
            if (key == 224 || key == 0) {
                key = _getch();
                // Player 1 (Arrow Keys)
                switch (key) {
                    case 75: if (snake1.getDirection() != Snake::RIGHT) snake1.setDirection(Snake::LEFT); break;
                    case 77: if (snake1.getDirection() != Snake::LEFT) snake1.setDirection(Snake::RIGHT); break;
                    case 72: if (snake1.getDirection() != Snake::DOWN) snake1.setDirection(Snake::UP); break;
                    case 80: if (snake1.getDirection() != Snake::UP) snake1.setDirection(Snake::DOWN); break;
                }
            } else {
                switch (key) {
                    // Player 2 (WASD)
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
};
```

---

## Summary Metrics
- **Attempt that worked**: Attempt 1 (first implementation worked after parameter alignment).
- **Total number of prompts to working code**: 1
- **What code was given to LLM**: Whole file (`snake_game.cpp`).
