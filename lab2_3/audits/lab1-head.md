# Code Smells Audit — lab1-head branch

Audit of `snake_game.cpp` after Lab-1 / Commit 4 multiplayer implementation.

## Summary of Findings

| Smell | File & Line | Category | Confidence | Description & Impact |
| --- | --- | --- | --- | --- |
| **Large Class** | `snake_game.cpp:164` | Bloaters | C4 | `SnakeGame` still manages rendering, user input, multi-snake game loop logic, and file storage persistence in one class. |
| **Primitive Obsession** | `snake_game.cpp:26` | Bloaters | C4 | Coordinates are still represented as raw `pair<int, int>` tuples throughout `Snake`, `Food`, and `Obstacle`. |
| **Data Clumps** | `snake_game.cpp:95` | Bloaters | C3 | `const vector<Snake>& snakes` and `const vector<pair<int, int>>& obstacles` are passed together to `Food::generate()` (`snake_game.cpp:95`) and `Obstacle::generate()` (`snake_game.cpp:130`). |
| **Long Method** | `snake_game.cpp:297` | Bloaters | C3 | `SnakeGame::logic()` processes dual snake movement, wall collisions, self-collisions, cross-snake collisions, obstacle collisions, and food scoring in 30 lines. |
| **Long Method** | `snake_game.cpp:268` | Bloaters | C3 | `SnakeGame::input()` handles multi-key parsing, arrow key escape sequences, WASD controls, pause, and exit signals in one method. |
| **Feature Envy** | `snake_game.cpp:311` | Couplers | C3 | `SnakeGame::logic()` directly accesses internals of `food.getPosition()` (`snake_game.cpp:316`) and `obstacle.isObstacle()` (`snake_game.cpp:311`) to determine state changes. |
| **Inappropriate Intimacy** | `snake_game.cpp:225` | Couplers | C3 | `SnakeGame::drawGame()` accesses internal coordinate vectors of `snake.getBody()` (`snake_game.cpp:225`) during rendering. |
| **Divergent Change** | `snake_game.cpp:164` | Change Preventers | C3 | `SnakeGame` requires changes whenever UI layout, storage format, keybindings, or multi-player collision rules change. |
| **Temporary Field** | `snake_game.cpp:174` | OO Abusers | C3 | `screenBuffer` remains declared as a persistent field of `SnakeGame` (`snake_game.cpp:174`) despite being used only transiently during `draw()`. |
