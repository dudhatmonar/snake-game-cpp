# 🐍 Snake Game in C++ (IT603 Final Project)

A modern, emoji-based console **Snake Game** built in **C++** using **Object-Oriented Programming (OOP)** principles.  
The game includes colorful fruit items, random obstacles, scoring and high-score saving, smooth screen updates, and adjustable difficulty.

---

## 🧩 Project Overview

This project was developed as part of the **IT603 – Final Project** to demonstrate:
- Understanding of **OOP concepts** (Encapsulation, Abstraction,..)
- Use of **file handling**, **keyboard input**, and **random number generation**
- Application of **game logic and collision detection** in C++

The game runs in the **Windows console** using emoji graphics for better visualization.

---

## 🚀 Features

- **🎮 Object-Oriented Design:**  
  Organized into classes `Snake`, `Food`, `Obstacle`, and `SnakeGame`.

- **🍎 Multiple Fruits with Points:**  
  Fruits like 🍎 🍇 🍓 🍊 🍌 🍍 each have different score values (10–40 points).

- **🧱 Random Obstacles:**  
  4–5 wall-style obstacles are placed randomly.  
  Hitting an obstacle ends the game immediately.

- **💯 Scoring System:**  
  Displays score, fruits eaten, and snake length.  
  High score is saved to a text file (`score.txt`).

- **⏸ Pause & Restart:**  
  Press `P` to pause, `R` to restart, `X` to exit.

- **⚡ Increasing Difficulty:**  
  Snake speed increases slightly every 5 fruits eaten.

- **Emoji Graphics**  
- Snake Head → 😎  
- Snake Body → 🟢  
- Wall/Obstacle → 🧱  
- Fruits → 🍎 🍇 🍓 🍊 🍌 🍍  

---

## 🕹️ Controls

| Key | Action |
|-----|---------|
| **W / ↑** | Move Up |
| **S / ↓** | Move Down |
| **A / ←** | Move Left |
| **D / →** | Move Right |
| **P** | Pause Game |
| **X** | Exit Game |
| **R** | Restart after Game Over |

---

## How to Compile
```bash
g++ -o snake_game snake_game.cpp
snake_game.exe
```

## Development Progress <br>
- Basic snake movement <br>
- Food generation and scoring <br>
- Emoji graphics implementation <br>
- Obstacle system <br>
- Game states and UI <br>

## Emoji Source
- https://github.com/enescakir/emoji/blob/master/constants.go

## Arrow Key Handling Explained
- Key 1: 224 or 0   ← "Hey, special key coming!"
- Key 2: 75  ( ← )  ← "This is the Left Arrow"
- Key 3: 77  ( → )  ← "This is the Right Arrow"
- Key 4: 72  ( ↑ )  ← "This is the Up Arrow"
- Key 5: 80  ( ↓ )  ← "This is the Down Arrow"


---

## ⚙️ Classes Overview

### 🐍 Snake
- Handles snake’s body, direction, and movement.
- **Functions:** `reset()`, `move()`, `isBody()`, `checkCollision()`, `getHead()`, `setDirection()`

### 🍎 Food
- Generates random fruit positions and points.
- **Functions:** `generate()`, `getPosition()`, `getSymbol()`, `getPoints()`

### 🧱 Obstacle
- Spawns 4–5 random obstacles that end the game on collision.
- **Functions:** `generate()`, `isObstacle()`, `getBlocks()`

### 🎮 SnakeGame
- Manages game logic, drawing, input, and scoring.
- **Functions:**  
  `draw()`, `logic()`, `input()`, `pauseGame()`, `showGameOver()`, `run()`

---

## 💾 File Handling

- **File Used:** `score.txt`
- **Purpose:** Stores the high score persistently.
- **Functions Used:**
  - `loadHighScore()` → Reads saved high score.
  - `saveHighScore()` → Updates file when new record is achieved.

---

## 🧮 Game Logic Flow

- **Step 1:** Initialize snake, food, and obstacles.  
- **Step 2:** Start main loop (`draw()`, `input()`, `logic()`).  
- **Step 3:** Detect collisions:  
  - Hitting wall → Game Over.  
  - Hitting self → Game Over.  
  - Hitting obstacle (🧱) → Game Over.  
- **Step 4:** Eat food → Grow snake, increase score, and slightly increase speed.  
- **Step 5:** On Game Over → Show stats, save score, and offer restart option.

---

## 🧠 OOP Concepts Demonstrated

- **Encapsulation:** Each class hides its internal data.  
- **Abstraction:** Game interacts only with class methods, not internals.  
- **Modularity:** Each game element (Snake, Food, Obstacle) is independent.  
- **File Handling:** Used for reading/writing high scores.  
- **Extensibility:** New fruits or features can easily be added.

---







