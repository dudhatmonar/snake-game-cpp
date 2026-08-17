# Code Smells Audit — main branch

Audit of `snake_game.cpp` at commit `main` before refactoring.

## Summary of Findings

| Smell | File & Line | Category | Confidence | Description & Impact |
| --- | --- | --- | --- | --- |
| **Shotgun Surgery** | `snake_game.cpp:166` | Change Preventers | C4 | The assumption of a single `Snake` instance is hardcoded across `SnakeGame`, `Food::generate()`, `Obstacle::generate()`, `drawGame()`, `input()`, `logic()`, and `showGameOver()`. Changing snake representation requires editing 8+ distinct functions. |
| **Large Class** | `snake_game.cpp:164` | Bloaters | C4 | `SnakeGame` handles rendering, user input, collision logic, game loop lifecycle, and file I/O persistence, violating the Single Responsibility Principle. |
| **Primitive Obsession** | `snake_game.cpp:26` | Bloaters | C4 | Coordinate positions are represented using raw `pair<int, int>` tuples throughout `Snake`, `Food`, and `Obstacle` instead of a dedicated `Point` abstraction. |
| **Data Clumps** | `snake_game.cpp:95` | Bloaters | C3 | `const vector<pair<int, int>>& snakeBody` and `const vector<pair<int, int>>& obstacles` are passed together to `Food::generate()` (`snake_game.cpp:95`) and `Obstacle::generate()` (`snake_game.cpp:130`). |
| **Long Method** | `snake_game.cpp:297` | Bloaters | C3 | `SnakeGame::logic()` combines movement, wall collision, self-collision, obstacle collision, food consumption, score updating, and speed adaptation in a single 28-line block. |
| **Long Method** | `snake_game.cpp:268` | Bloaters | C3 | `SnakeGame::input()` handles multi-key parsing, arrow key escape codes, WASD inputs, pause commands, and exit triggers in one block. |
| **Feature Envy** | `snake_game.cpp:311` | Couplers | C3 | `SnakeGame::logic()` directly accesses internals of `food.getPosition()` (`snake_game.cpp:316`) and `obstacle.isObstacle()` (`snake_game.cpp:311`) to drive state transitions rather than delegating entity interaction. |
| **Inappropriate Intimacy** | `snake_game.cpp:225` | Couplers | C3 | `SnakeGame::drawGame()` inspects internal coordinate arrays of `snake.getBody()` (`snake_game.cpp:225`) directly during grid drawing. |
| **Duplicate Code** | `snake_game.cpp:274` | Dispensables | C3 | Direction checking and setting logic is duplicated between arrow key branches (`snake_game.cpp:274-277`) and WASD key branches (`snake_game.cpp:281-284`). |
| **Divergent Change** | `snake_game.cpp:164` | Change Preventers | C3 | `SnakeGame` must be modified whenever UI layout changes, file persistence format changes, keybindings change, or collision rules change. |
| **Temporary Field** | `snake_game.cpp:174` | OO Abusers | C3 | `screenBuffer` is declared as a persistent member variable of `SnakeGame` (`snake_game.cpp:174`) despite only being used transiently within `draw()`. |
| **Lazy Class** | `snake_game.cpp:124` | Dispensables | C1 | `Obstacle` class primarily wraps a `vector<pair<int, int>>` and a simple search method `isObstacle()`, adding minimal behavior. |
