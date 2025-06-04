#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Ball.hpp"
#include "Paddle.hpp"
#include "Background.hpp"


enum class GameState { Menu, Playing, Exit };
enum class Difficulty { Easy, Medium, Hard };
enum class GameMode { PvAI, PvP };

class Game {
public:
    Game();
    void start();
    void setupMenu();
    void drawMenu();
    void setupGame();
    void saveScores();
    void loadScores();

private:
    sf::RenderWindow window;
    sf::Vector2f randomBallVelocity(float speed = 350.f);
    Ball ball;
    Ball ball2;
    Paddle playerPaddle;
    Paddle aiPaddle;
    Background background;
    bool isRunning;

    int aiLevel = 1;
    int player1Hits = 0;
    int player2Hits = 0;
    int player1Score = 0;
    int player2Score = 0;
    const int maxScore = 5;
    int ballsCount = 1;

    void processInput(float dt);
    void update(float dt);
    void resetRound();
    void showWinner(int winner);
    GameState state = GameState::Menu;
    Difficulty difficulty = Difficulty::Easy;
    GameMode mode = GameMode::PvAI;
    int menuSelected = 0; // 0 - PvAI, 1 - PvP, 2 - Difficulty, 3 - Exit
    bool smallPaddles = false;
};

#endif // GAME_HPP