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

## Controls
- W / ↑ - Move Up
- A / ← - Move Left
- S / ↓ - Move Down
- D / → - Move Right
- P - Pause
- X - Exit

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

## Classes Overview
- Class -> Purpose                                                                                       <br>
- Snake -> Handles the snake’s body, movement, direction, and collision detection.               <br>
- Food  -> Handles generating food (fruit) at random positions with symbols and points.                 <br>
- Obstacle -> Manages obstacle generation and checking for collisions.                                     <br>
- SnakeGame -> Main controller — manages game logic, rendering, and interactions between all other classes. <br>




