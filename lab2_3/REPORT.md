# Lab 2_3 — Group A01

---

## 1. Tool and install route — [3]

| | |
|---|---|
| Agent used for run 2 | Gemini 3.6 Flash (High) via Antigravity |
| `ubiquitous-language` install route | Project `.claude/skills` / Antigravity session skill loading |
| `refactoring/` pack install route | Project `.claude/skills` / Antigravity session skill loading |

Skills were installed into the project skills directory and loaded during the session.

---

## 2. What I changed in the glossary — [4]

The generated file is at `lab2_3/UBIQUITOUS_LANGUAGE.md`. We refined the generated definitions to clarify the relationship between `Food` (the class entity) and `Fruit` (the item variety), explicitly defined `Grid` bounds (`WIDTH` / `HEIGHT`) and `Obstacle` hazards (`💀`), and recorded code drift where global constants are used instead of domain objects.

---

## 3. Smell delta — [6]

Reports: `lab2_3/audits/main.md` (the code as you received it) and `lab2_3/audits/lab1-head.md` (after your Lab-1 PR).

| | count | representative site (`file:line`) |
|---|---|---|
| Smells my Lab-1 PR **introduced** | 0 | N/A |
| Smells my Lab-1 PR **left untouched** | 9 | `snake_game.cpp:164` |
| Smells my Lab-1 PR **removed** | 2 | `snake_game.cpp:166` |

---

## 4. Rejected candidates — [6]

At least three things the agent reported that are *not* real findings on this codebase.

| smell reported | `file:line` | why it does not hold |
|---|---|---|
| Lazy Class | `snake_game.cpp:124` | `Obstacle` encapsulates block storage, random spawn generation, and collision checking, isolating hazard concerns from `SnakeGame`. |
| Duplicate Code | `snake_game.cpp:363` | `setupConsole()` executes Windows terminal initialization (`chcp 65001` and cursor hiding) once at startup and cannot be refactored into a class hierarchy. |
| Large Class | `snake_game.cpp:24` | `Snake` only manages body coordinates, direction state, movement, and self-collision, which strictly belong to a single domain entity. |

---

## 5. Commit map — [7]

Run `lab2_3/check-lab2_3.sh` and paste the table it prints.

| # | sha | subject | what it is |
|---|---|---|---|
| 1 | f84e9b2 | Create ubiquitous language glossary | glossary |
| 2 | 0ac517b | Add code smells audit for main | smell report |
| 3 | 4858daf | Refactor snake count encapsulation | **the refactor, alone** |
| 4 | a3849b8 | Add second player | **the feature, alone** |

---

## 6. Two-run measurement — [4]

Run 1 is your Lab-1 branch — the numbers you already reported. Run 2 is commit 4 alone.

| | Run 1 (Lab 1) | Run 2 (commit 4) |
|---|---|---|
| Smells introduced | 1 | 0 |
| Lines changed, `git diff --shortstat -w` | 1 file changed, 109 insertions(+), 58 deletions(-) | 1 file changed, 64 insertions(+), 36 deletions(-) |
| Lines changed, **raw** (no `-w`) | 1 file changed, 109 insertions(+), 58 deletions(-) | 1 file changed, 71 insertions(+), 43 deletions(-) |
| Functions reached | 12 | 3 |
| Prompts to working code | 1 | 1 |
| Wall-clock time | 0.5 hours | 0.15 hours |

Commit 3 (the refactor) on its own: 1 file changed, 64 insertions(+), 31 deletions(-) `-w`, 1 file changed, 87 insertions(+), 54 deletions(-) raw.

---

## 7. Analysis Q1–Q2 — [5]

**Q1. Which smell did commit 3 actually fix?** Name it from your section 3 report. What was expensive before, what does it cost now.

Commit 3 fixed `Shotgun Surgery` (`snake_game.cpp:166`). Before the refactor, modifying snake representation or adding a second player required editing 8+ distinct functions across `SnakeGame`, `Food`, and `Obstacle`. By encapsulating snake management inside a `vector<Snake> snakes` collection in `SnakeGame` and passing `snakes` to `Food` and `Obstacle`, snake count is now defined in exactly one place (`resetGame()`). Adding or modifying snakes now costs a single vector entry rather than cross-class refactoring.

**Q2. Compare commit 4 to your Lab-1 diff.** Same feature, same codebase. What changed in the cost and what did not? If it got worse, say so and explain — that marks the same.

In Lab 1, adding the second player required editing rendering loops, key input switches, collision checks, spawn logic, and scoreboard displays across 12 functions and 167 lines. In Run 2 (Commit 4), because Commit 3 had already decoupled rendering and collision from single-snake assumptions, adding Player 2 required only initializing a second `Snake` in `resetGame()` and mapping WASD controls. The diff cost dropped from 12 functions to 3 functions and from 109 insertions to 64 insertions (-w).

---

## 8. Analysis Q3–Q4 — [5]

**Q3. Go back through your Lab-1 `LLM-LOG.md`. Did the assistant ever suggest restructuring before adding the feature?** Quote it if it did. If it did not, what would have had to be different in your prompt?

The assistant did not suggest restructuring before adding the feature; it directly implemented 2-player multiplayer on the existing codebase structure. To prompt a design-first approach, we would have needed to explicitly instruct: *"Before adding the second player, audit the codebase for single-snake assumptions and refactor the architecture so snake collection management is encapsulated in exactly one place."*

**Q4. How do you know commit 3 did not change behaviour?** Answer honestly. Most of you will find that you do not know. Say that plainly if it is true, and describe what you would have needed in order to actually know.

We verified that Commit 3 did not change behavior by compiling and manually playtesting `snake_game.exe`. In single-player mode, the snake spawned at `(WIDTH/2, HEIGHT/2)`, responded to inputs, ate food, triggered obstacles, and ended the game identically to `main`. However, manual playtesting cannot guarantee complete absence of regression. An automated suite of unit and integration tests covering movement, collision edge cases, and scoring would be required to formally verify zero behavioral change.

---

## If you did not finish

N/A — All steps completed successfully.
