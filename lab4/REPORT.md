# Lab 4 — Group A06

| | |
|---|---|
| Repository | `https://github.com/dudhatmonar/snake-game-cpp` |
| Base tag | `lab4-base` at commit `f9428f408be81893f22de5f4a737e1f4f2286b9a` |
| Pull request | `lab4` → `main` (Title: `Lab 4: Fruit consumption increases score and grows snake`) |

---

## 1. Five rules — [5]

Written before opening the source. Behaviour, with an observable outcome.

| # | Rule |
|---|---|
| 1 | The snake dies (`gameOver` becomes `true`) when its head collides with any part of its own body. |
| 2 | The snake dies (`gameOver` becomes `true`) when its head moves outside the grid boundaries (`x < 0`, `x >= WIDTH`, `y < 0`, or `y >= HEIGHT`). |
| 3 | The snake dies (`gameOver` becomes `true`) when its head collides with an obstacle coordinate. |
| 4 | Food never appears on a cell occupied by any segment of the snake's body or an obstacle block. |
| 5 | When the snake's head reaches the food position, the score increases by the food's point value, the fruit counter increments, and the snake grows in length by 1. |

---

## 2. What you could test, and what stopped you — [10]

No source changes in this part. Every `file:line` below is a line in `lab4-base`.

| # | Rule | Test written? | Blocking dependency (`file:line` + what it is) |
|---|---|---|---|
| 1 | Snake dies on self-collision | No | `snake_game.cpp:166,173` — `Snake snake` and `bool gameOver` are private fields in `SnakeGame` with no public getters; `snake_game.cpp:344,352` — `SnakeGame::run()` hardcodes blocking `_getch()` and `Sleep(speed)` inside the game loop preventing automated headless execution. |
| 2 | Snake dies on boundary collision | No | `snake_game.cpp:173` — `gameOver` state is private within `SnakeGame` with no inspection method; `snake_game.cpp:340-359` — `SnakeGame::run()` couples the game loop with interactive console I/O (`_getch()`, `Sleep()`) and provides no public programmatic `step()` / `tick()` method. |
| 3 | Snake dies on obstacle collision | No | `snake_game.cpp:137-138` — `Obstacle::generate()` directly invokes non-deterministic `rand()` to place obstacles randomly without dependency injection; `snake_game.cpp:168,173` — `obstacle` and `gameOver` are private fields in `SnakeGame`. |
| 4 | Food never spawns on snake or obstacle | Yes | None (testable via unit test directly invoking `Food::generate(snakeBody, obstacles)` and checking position non-overlap). |
| 5 | Eating fruit increases score and grows snake | No | `snake_game.cpp:99,112` — `Food::generate()` uses unseeded `rand()` for position and fruit type/points selection; `snake_game.cpp:167,169,171` — `food`, `score`, and `fruitsEaten` are private fields inside `SnakeGame` with no setter, getter, or injectable food supplier. |

> **Rules testable without modifying the source: 1 / 5**

---

## 3. Coverage, and what it missed — [6]

| | |
|---|---|
| Line coverage | 6.64 % (of 241 lines in `snake_game.cpp`) |
| Branch coverage | 9.96 % (of 281 branches in `snake_game.cpp`, 6.76% taken at least once) |
| Command used | `g++ -std=c++11 --coverage -O0 -g -Dmain=game_main tests_part_b.cpp -o tests_part_b && ./tests_part_b && gcov -b tests_part_b-snake_game.cpp snake_game.cpp` |

**One rule that is executed by the suite but not verified by it:**

| | |
|---|---|
| Rule | Food point value assignment based on randomly selected fruit type |
| Line that runs | `snake_game.cpp:114` (`points = fruits[fruitIndex].second;` executed 100 times during `food.generate()`) |
| The assertion that is missing | `assert(food.getPoints() >= 10 && food.getPoints() <= 40);` (and verifying `food.getSymbol()` corresponds to the expected fruit point tier) |

---

## 4. The seam — [10]

| | |
|---|---|
| Rule made testable | Rule 5: When the snake's head reaches the food position, the score increases by the food's point value, the fruit counter increments, and the snake grows in length by 1 |
| Commit 1 (seam) | `d1bd8859d99f0fda79dfed1de1b72ebea84f4e21` |
| Commit 2 (test) | `d1366b62952df5ddb0e87ee95e4d45dd2113dc7e` |
| Seam kind | object |
| Enabling point | `SnakeGame` constructor parameter accepting an optional `IFood*` collaborator (`SnakeGame(IFood* customFood = nullptr)`), alongside public headless step and state observer methods |
| What production code gave up | Production code gave up hardcoded concrete instantiation of the `Food` dependency inside `SnakeGame`. By depending on the `IFood` abstraction rather than directly generating random coordinates, the production code relinquished exclusive internal control of food provisioning, allowing tests to inject deterministic food coordinates and point values while preserving 100% identical gameplay behavior for human players via the default constructor parameter fallback. |

---

## 5. The double — [4]

| | |
|---|---|
| What you passed through the seam | Stub (`StubFood`) |
| The method under test | `SnakeGame::logic()` / `SnakeGame::step()` |

Two sentences: was the collaborator asked a question or told to do something, and why does that decide the answer above?

The collaborator (`StubFood`) was **asked a question**: during `SnakeGame::logic()`, the game queries it for its location via `food->getPosition()` and its point value via `food->getPoints()` to compute collisions and increment the score. Because the collaborator supplies predetermined, canned answers to state queries rather than verifying side-effect command invocations, it is classified as a **Stub**.

---

## 6. Two smells in your own tests — [5]

| | Smell | `file:line` | One-line fix |
|---|---|---|---|
| 1 | Assertion roulette | `tests/test_snake.cpp:55` | Split the multi-attribute assertion block into distinct sub-assertions with explicit failure messages (`assert(game.getScore() == 25 && "Score did not match fruit points");`). |
| 2 | Mystery guest | `tests/test_snake.cpp:42` | Explicitly arrange initial snake starting coordinates and orientation inside the test body instead of relying on implicit default constructor coordinates. |

---
