# Ubiquitous Language

## Game Core

| Term | Definition | Aliases to avoid | In code |
| --- | --- | --- | --- |
| **Snake** | Player-controlled entity consisting of a head and body segments moving across the grid | Player, worm, crawler | `Snake` — `snake_game.cpp:24` |
| **Food** | Collectible item generated randomly on the grid that awards points and grows the snake | Fruit, pickup, item | `Food` — `snake_game.cpp:83` |
| **Fruit** | Specific fruit variety with assigned point value (10–40 points) | Symbol, reward | `fruits` — `snake_game.cpp:89` |
| **Obstacle** | Randomly placed hazard block on the grid causing instant Game Over on collision | Wall, rock, trap, block | `Obstacle` — `snake_game.cpp:124` |
| **Grid** | Bounded playing field surrounded by wall boundaries | Board, arena, field, screen | `WIDTH`, `HEIGHT` — `snake_game.cpp:12-13` |
| **Direction** | Spatial movement orientation of the snake | Heading, vector, key | `Direction` — `snake_game.cpp:30` |
| **Score** | Total points accumulated by eating fruits during a game session | Points, total | `score` — `snake_game.cpp:169` |
| **High Score** | Highest score recorded across game sessions, persisted to disk | Record, top score, best | `highScore` — `snake_game.cpp:172` |

## Lifecycle & Events

| Term | Definition | Aliases to avoid | In code |
| --- | --- | --- | --- |
| **Collision** | Contact between snake head and wall, obstacle, or snake body triggering Game Over | Impact, crash, hit | `checkCollision()` — `snake_game.cpp:71` |
| **Game Over** | Termination state when collision occurs or exit key is pressed | End, death, loss | `gameOver` — `snake_game.cpp:173` |
| **Speed** | Frame delay controlling snake movement velocity (decreases as fruits are eaten) | Delay, tempo, rate | `speed` — `snake_game.cpp:170` |

## Relationships

- A **Grid** contains one or more **Snakes**, one **Food**, and multiple **Obstacles**
- A **Snake** eats **Food**, which increases **Score** and **Snake** length
- A **Collision** with a wall, **Obstacle**, or **Snake** body triggers **Game Over**
- A new **High Score** persists to file storage upon **Game Over**

## Example dialogue

> **Dev:** "When a **Snake** moves onto the cell containing **Food**, do we immediately spawn a new **Food** item?"  
> **Domain expert:** "Yes, eating **Food** adds to the player's **Score**, increases the **Snake** length, and generates a new **Food** item and **Obstacles** on unoccupied **Grid** positions."  
> **Dev:** "What happens if the **Snake** collides with an **Obstacle**?"  
> **Domain expert:** "Any **Collision** with an **Obstacle**, outer wall, or self triggers **Game Over** immediately."

## Flagged ambiguities

- "Fruit" vs "Food": Code uses `Food` class name but stores internal list named `fruits` and counts `fruitsEaten`. `Food` is the canonical game object concept; `Fruit` represents the specific collectible item variant.
- "Obstacle" vs "Wall": The code uses `🧱` for perimeter walls and `💀` for internal `Obstacle` blocks, but both trigger collision in `logic()`.

## Code drift

| Canonical term | Called in code | Location | Note |
| --- | --- | --- | --- |
| **Food** | `Food` / `fruits` | `snake_game.cpp:83,89` | Class is `Food`, internal vector is `fruits`, stats call it `fruitsEaten` |
| **Grid** | `WIDTH` / `HEIGHT` | `snake_game.cpp:12-13` | Grid boundaries exist as global constants rather than a `Grid` class |
| **Collision** | `checkCollision()` / `isObstacle()` | `snake_game.cpp:71,152` | Collision logic is split across `Snake`, `Obstacle`, and `SnakeGame` |
