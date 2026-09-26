#include <iostream>
#include <cassert>
#include <vector>
#include <utility>
#include <string>

// Include snake_game source via macro seam to avoid multiple main definitions
#define main game_main
#include "../snake_game.cpp"
#undef main

// =========================== Test Double: StubFood ===========================
// StubFood is a Stub: it responds to queries (getPosition, getPoints, getSymbol)
// with canned values preconfigured by the test.
class StubFood : public IFood {
private:
    pair<int, int> cannedPosition;
    int cannedPoints;
    string cannedSymbol;

public:
    StubFood(pair<int, int> pos, int points, string symbol = "🍎")
        : cannedPosition(pos), cannedPoints(points), cannedSymbol(symbol) {}

    void generate(const vector<pair<int, int>>& /*snakeBody*/, const vector<pair<int, int>>& /*obstacles*/) override {
        // Canned response: keep the pre-set position for deterministic collision testing
    }

    pair<int, int> getPosition() const override { return cannedPosition; }
    string getSymbol() const override { return cannedSymbol; }
    int getPoints() const override { return cannedPoints; }

    void setPosition(pair<int, int> pos) { cannedPosition = pos; }
    void setPoints(int pts) { cannedPoints = pts; }
};

// =========================== Test Suite ===========================

// Test for Rule 5: Fruit Consumption Growth and Scoring (Made testable via Object Seam)
void test_rule_5_fruit_consumption_increases_score_and_grows_snake() {
    // Arrange: Snake starts at head (25, 12) moving right into food placed at (26, 12)
    StubFood stubFood({26, 12}, 25, "🍊");
    SnakeGame game(&stubFood);

    assert(game.getScore() == 0);
    assert(game.getFruitsEaten() == 0);
    assert(game.getSnakeLength() == 3);
    assert(!game.isGameOver());

    // Act: Move snake RIGHT onto food cell (26, 12)
    game.setDirection(Snake::RIGHT);
    game.step();

    // Assert: Fruit consumed, score updated, length increased, head advanced
    assert(game.getSnakeHead().first == 27 && game.getSnakeHead().second == 12);
    assert(game.getScore() == 25);
    assert(game.getFruitsEaten() == 1);
    assert(game.getSnakeLength() == 4);
    assert(!game.isGameOver());

    cout << "[PASS] Rule 5: Fruit consumption increases score and grows snake." << endl;
}

// Test for Rule 2: Boundary Collision Game Over
void test_rule_2_wall_collision_ends_game() {
    StubFood stubFood({-10, -10}, 10);
    SnakeGame game(&stubFood);

    game.setDirection(Snake::RIGHT);
    // Head starts at x=25, grid width is 50 (valid range 0..49). Move 26 steps right to exceed boundary.
    for (int i = 0; i < 26; ++i) {
        game.step();
    }

    assert(game.isGameOver() == true);
    cout << "[PASS] Rule 2: Snake dies when moving outside grid boundary." << endl;
}

// Test for Rule 4: Food Generation Non-Overlap
void test_rule_4_food_never_overlaps_snake_or_obstacles() {
    Food realFood;
    vector<pair<int, int>> snakeBody = { {25, 12}, {24, 12}, {23, 12} };
    vector<pair<int, int>> obstacles = { {10, 10}, {15, 15}, {20, 20} };

    for (int i = 0; i < 50; ++i) {
        realFood.generate(snakeBody, obstacles);
        pair<int, int> pos = realFood.getPosition();
        for (const auto& s : snakeBody) {
            assert(pos != s);
        }
        for (const auto& o : obstacles) {
            assert(pos != o);
        }
    }
    cout << "[PASS] Rule 4: Food never overlaps snake or obstacles." << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "Running Snake Game Test Suite (Lab 4)..." << endl;
    cout << "========================================" << endl;

    test_rule_5_fruit_consumption_increases_score_and_grows_snake();
    test_rule_2_wall_collision_ends_game();
    test_rule_4_food_never_overlaps_snake_or_obstacles();

    cout << "========================================" << endl;
    cout << "All automated tests passed successfully!" << endl;
    cout << "========================================" << endl;
    return 0;
}
