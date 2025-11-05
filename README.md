# Snake Game in C++

A terminal-based Snake game built with C++ featuring emoji graphics and dynamic obstacles.

## Features
- 🐍 Emoji snake with colored body segments
- 🍎 Multiple fruit types with different points
- 💀 Temporary obstacles that appear and disappear
- 🎯 Score system and progressive difficulty
- ⌨️ Keyboard controls (WASD + Arrow keys)

## How to Compile
```bash
g++ -o snake_game snake_game.cpp
./snake_game.exe
```

## Controls
- W / ↑ - Move Up
- A / ← - Move Left
- S / ↓ - Move Down
- D / → - Move Right
- P - Pause
- X - Exit

## Development Progress <br>
✅ Basic snake movement <br>
✅ Food generation and scoring <br>
✅ Emoji graphics implementation <br>
✅ Obstacle system <br>
✅ Game states and UI <br>

## Emoji Source
- https://github.com/enescakir/emoji/blob/master/constants.go

## Arrow Key Handling Explained
- Key 1: 224 or 0   ← "Hey, special key coming!"
- Key 2: 75  ( ← )  ← "This is the Left Arrow"
- Key 3: 77  ( → )  ← "This is the Right Arrow"
- Key 4: 72  ( ↑ )  ← "This is the Up Arrow"
- Key 5: 80  ( ↓ )  ← "This is the Down Arrow"

