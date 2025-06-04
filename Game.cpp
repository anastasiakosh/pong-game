#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include <sstream>
#include <random>
#include <cmath>
#include <fstream>
#include "paddle/PaddleActions.hpp"

void Game::saveScores() {
    std::ofstream out("scores.txt");
    if (out) {
        out << player1Hits << " " << player2Hits << std::endl;
    }
}

void Game::loadScores() {
    std::ifstream in("scores.txt");
    if (in) {
        in >> player1Hits >> player2Hits;
    }
}

Game::Game()
    : ball(400.f, 300.f),
      ball2(400.f, 300.f),
      playerPaddle(30.f, 300.f),
      aiPaddle(760.f, 300.f),
      background(),
      window(sf::VideoMode(800, 600), "Pong Game"),
      isRunning(true)
{
    // score всегда сбрасывается при запуске
    player1Hits = 0;
    player2Hits = 0;
    saveScores();
}

void Game::start() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // Меню
        if (state == GameState::Menu) {
            drawMenu();
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up)
                        menuSelected = (menuSelected + 2) % 3;
                    if (event.key.code == sf::Keyboard::Down)
                        menuSelected = (menuSelected + 1) % 3;
                    if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Return) {
                        if (menuSelected == 0) {
                            mode = GameMode::PvAI;
                            aiLevel = 1;
                            player1Hits = 0; // сбросить score при старте режима
                            player2Hits = 0;
                            saveScores();
                            setupGame();
                            state = GameState::Playing;
                        }
                        if (menuSelected == 1) {
                            mode = GameMode::PvP;
                            player1Hits = 0; // сбросить score при старте режима
                            player2Hits = 0;
                            saveScores();
                            setupGame();
                            state = GameState::Playing;
                        }
                        if (menuSelected == 2) { window.close(); }
                    }
                }
            }
            continue;
        }

        // --- Игровой процесс ---
        if (state == GameState::Playing) {
            // Обработка Esc во время игры
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape)
                        window.close();
                }
            }

            playerPaddle.setDY(0);
            aiPaddle.setDY(0);

            if (mode == GameMode::PvP) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                    playerPaddle.setDY(-300.f);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                    playerPaddle.setDY(300.f);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                    aiPaddle.setDY(-300.f);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                    aiPaddle.setDY(300.f);
            } else if (mode == GameMode::PvAI) {
                float aiSpeed = 200.f;
                if (aiLevel == 1) aiSpeed = 220.f;
                if (aiLevel == 2) aiSpeed = 340.f;
                if (aiLevel == 3) aiSpeed = 420.f;

                float ballY = ball.getBounds().top + ball.getBounds().height / 2.f;
                float paddleY = playerPaddle.getBounds().top + playerPaddle.getBounds().height / 2.f;

                if (std::abs(paddleY - ballY) > 10) {
                    if (paddleY < ballY)
                        playerPaddle.setDY(aiSpeed);
                    else
                        playerPaddle.setDY(-aiSpeed);
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                    aiPaddle.setDY(-300.f);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                    aiPaddle.setDY(300.f);
            }

            playerPaddle.move(playerPaddle.getDY() * dt, dt);
            aiPaddle.move(aiPaddle.getDY() * dt, dt);

            float ballSpeed = 350.f;
            if (mode == GameMode::PvAI && aiLevel == 3) ballSpeed = 500.f;
            ball.move(dt);
            if (ballsCount == 2) ball2.move(dt);

            auto handleBall = [&](Ball& b) {
                if (b.getBounds().intersects(playerPaddle.getBounds()) && b.getVelocity().x < 0) {
                    b.setVelocity(-b.getVelocity().x, b.getVelocity().y);
                    player1Hits += 10;
                    saveScores();
                }
                if (b.getBounds().intersects(aiPaddle.getBounds()) && b.getVelocity().x > 0) {
                    b.setVelocity(-b.getVelocity().x, b.getVelocity().y);
                    player2Hits += 10;
                    saveScores();
                }

                sf::Vector2f pos = b.getBounds().getPosition();
                if (pos.x < 0) {
                    player2Score++;
                    if (player2Score >= maxScore) { showWinner(2); }
                    else resetRound();
                }
                if (pos.x > 800 - b.getBounds().width) {
                    player1Score++;
                    if (player1Score >= maxScore) { showWinner(1); }
                    else resetRound();
                }
            };

            handleBall(ball);
            if (ballsCount == 2) handleBall(ball2);
        }

        window.clear(sf::Color::Black);
        background.draw(window);
        ball.draw(window);
        if (ballsCount == 2) ball2.draw(window);
        playerPaddle.draw(window);
        aiPaddle.draw(window);

        sf::Font font;
        font.loadFromFile("arial.ttf");

        sf::Text scoreText;
        std::stringstream ss;
        ss << player1Score << " : " << player2Score;
        scoreText.setFont(font);
        scoreText.setString(ss.str());
        scoreText.setCharacterSize(40);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(350, 10);
        window.draw(scoreText);

        sf::Text leftScore, rightScore;
        leftScore.setFont(font);
        rightScore.setFont(font);
        leftScore.setString("score: " + std::to_string(player1Hits));
        rightScore.setString("score: " + std::to_string(player2Hits));
        leftScore.setCharacterSize(24);
        rightScore.setCharacterSize(24);
        leftScore.setFillColor(sf::Color::White);
        rightScore.setFillColor(sf::Color::White);
        leftScore.setPosition(30, 10);
        rightScore.setPosition(670, 10);
        window.draw(leftScore);
        window.draw(rightScore);

        window.display();
    }
}

void Game::setupGame() {
    ballsCount = 1;
    smallPaddles = false;
    ball.reset();
    playerPaddle.reset(30.f, 300.f);
    aiPaddle.reset(760.f, 300.f);

    if (mode == GameMode::PvP) {
        if (difficulty == Difficulty::Medium) {
            playerPaddle.setSize(10, 60);
            aiPaddle.setSize(10, 60);
        } else if (difficulty == Difficulty::Hard) {
            ballsCount = 2;
            ball.reset();
            ball2.reset();
            float speed = 350.f;
            float angle1 = (std::rand() % 60 - 30) * 3.14159265f / 180.f;
            float angle2 = (std::rand() % 60 - 30) * 3.14159265f / 180.f;
            sf::Vector2f v1(speed * std::cos(angle1), speed * std::sin(angle1));
            sf::Vector2f v2(speed * std::cos(angle2), speed * std::sin(angle2));
            ball.setVelocity(v1.x, v1.y);
            ball2.setVelocity(v2.x, v2.y);
        }
    }
    if (mode == GameMode::PvAI && aiLevel == 3) {
        float speed = 500.f;
        float angle = (std::rand() % 60 - 30) * 3.14159265f / 180.f;
        sf::Vector2f v(speed * std::cos(angle), speed * std::sin(angle));
        ball.setVelocity(v.x, v.y);
    }
}

void Game::drawMenu() {
    window.clear(sf::Color::Black);
    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text title("PONG", font, 60);
    title.setFillColor(sf::Color::White);
    title.setPosition(300, 60);
    window.draw(title);

    std::vector<std::string> items = {
        "Play vs AI",
        "Play vs Player",
        "Exit"
    };

    for (int i = 0; i < items.size(); ++i) {
        sf::Text item(items[i], font, 36);
        item.setFillColor(i == menuSelected ? sf::Color::Yellow : sf::Color(180,180,180));
        sf::FloatRect bounds = item.getLocalBounds();
        item.setOrigin(bounds.width / 2, 0);
        item.setPosition(400, 200 + i * 70);
        window.draw(item);
    }

    window.display();
}

void Game::resetRound() {
    ball.reset();
    if (ballsCount == 2) ball2.reset();
    playerPaddle.reset(30.f, 300.f);
    aiPaddle.reset(760.f, 300.f);
    if (ballsCount == 2) {
        auto v1 = randomBallVelocity();
        auto v2 = randomBallVelocity();
        ball.setVelocity(v1.x, v1.y);
        ball2.setVelocity(v2.x, v2.y);
    }
}

void Game::showWinner(int winner) {
    if (mode == GameMode::PvP && winner == 2) {
        if (difficulty == Difficulty::Easy) {
            window.clear();
            sf::Font font;
            font.loadFromFile("arial.ttf");
            sf::Text levelText("Level 2", font, 48);
            levelText.setFillColor(sf::Color::Yellow);
            sf::FloatRect rect = levelText.getLocalBounds();
            levelText.setOrigin(rect.width / 2, rect.height / 2);
            levelText.setPosition(400, 250);
            window.draw(levelText);
            window.display();
            sf::sleep(sf::seconds(2));

            difficulty = Difficulty::Medium;
            setupGame();
            player1Score = 0;
            player2Score = 0;
            state = GameState::Playing;
            return;
        } else if (difficulty == Difficulty::Medium) {
            window.clear();
            sf::Font font;
            font.loadFromFile("arial.ttf");
            sf::Text levelText("Level 3", font, 48);
            levelText.setFillColor(sf::Color::Yellow);
            sf::FloatRect rect = levelText.getLocalBounds();
            levelText.setOrigin(rect.width / 2, rect.height / 2);
            levelText.setPosition(400, 250);
            window.draw(levelText);
            window.display();
            sf::sleep(sf::seconds(2));

            difficulty = Difficulty::Hard;
            setupGame();
            player1Score = 0;
            player2Score = 0;
            state = GameState::Playing;
            return;
        } else if (difficulty == Difficulty::Hard) {
            window.clear();
            sf::Font font;
            font.loadFromFile("arial.ttf");
            sf::Text endText("Game Over!", font, 48);
            endText.setFillColor(sf::Color::Yellow);
            sf::FloatRect rect = endText.getLocalBounds();
            endText.setOrigin(rect.width / 2, rect.height / 2);
            endText.setPosition(400, 250);
            window.draw(endText);
            window.display();
            sf::sleep(sf::seconds(2));

            player1Hits = 0;
            player2Hits = 0;
            saveScores();

            difficulty = Difficulty::Easy;
            player1Score = 0;
            player2Score = 0;
            state = GameState::Menu;
            return;
        }
    }

    if (mode == GameMode::PvAI && winner == 2) {
        if (aiLevel == 1) {
            window.clear();
            sf::Font font;
            font.loadFromFile("arial.ttf");
            sf::Text levelText("Level 2", font, 48);
            levelText.setFillColor(sf::Color::Yellow);
            sf::FloatRect rect = levelText.getLocalBounds();
            levelText.setOrigin(rect.width / 2, rect.height / 2);
            levelText.setPosition(400, 250);
            window.draw(levelText);
            window.display();
            sf::sleep(sf::seconds(2));

            aiLevel = 2;
            setupGame();
            player1Score = 0;
            player2Score = 0;
            state = GameState::Playing;
            return;
        } else if (aiLevel == 2) {
            window.clear();
            sf::Font font;
            font.loadFromFile("arial.ttf");
            sf::Text levelText("Level 3", font, 48);
            levelText.setFillColor(sf::Color::Yellow);
            sf::FloatRect rect = levelText.getLocalBounds();
            levelText.setOrigin(rect.width / 2, rect.height / 2);
            levelText.setPosition(400, 250);
            window.draw(levelText);
            window.display();
            sf::sleep(sf::seconds(2));

            aiLevel = 3;
            setupGame();
            player1Score = 0;
            player2Score = 0;
            state = GameState::Playing;
            return;
        } else if (aiLevel == 3) {
            window.clear();
            sf::Font font;
            font.loadFromFile("arial.ttf");
            sf::Text endText("Game Over!", font, 48);
            endText.setFillColor(sf::Color::Yellow);
            sf::FloatRect rect = endText.getLocalBounds();
            endText.setOrigin(rect.width / 2, rect.height / 2);
            endText.setPosition(400, 250);
            window.draw(endText);
            window.display();
            sf::sleep(sf::seconds(2));

            player1Hits = 0;
            player2Hits = 0;
            saveScores();

            aiLevel = 1;
            player1Score = 0;
            player2Score = 0;
            state = GameState::Menu;
            return;
        }
    }

    if (winner == 1) {
        player1Hits = 0;
        player2Hits = 0;
        saveScores();
    }

    window.clear();
    sf::Font font;
    font.loadFromFile("arial.ttf");

    std::string winnerText = (winner == 1 ? "Player 1 wins!" : "Player 2 wins!");
    sf::Text winText(winnerText, font, 50);
    winText.setFillColor(sf::Color::Yellow);
    sf::FloatRect textRect = winText.getLocalBounds();
    winText.setOrigin(textRect.width / 2, textRect.height / 2);
    winText.setPosition(400, 220);
    window.draw(winText);

    sf::RectangleShape button(sf::Vector2f(220, 60));
    button.setFillColor(sf::Color(180, 180, 180));
    button.setOrigin(110, 30);
    button.setPosition(400, 350);
    window.draw(button);

    sf::Text btnText("В меню (Enter)", font, 32);
    btnText.setFillColor(sf::Color::Black);
    sf::FloatRect btnRect = btnText.getLocalBounds();
    btnText.setOrigin(btnRect.width / 2, btnRect.height / 2);
    btnText.setPosition(400, 350);
    window.draw(btnText);

    sf::Text leftScore("score: " + std::to_string(player1Hits), font, 24);
    sf::Text rightScore("score: " + std::to_string(player2Hits), font, 24);
    leftScore.setFillColor(sf::Color::White);
    rightScore.setFillColor(sf::Color::White);
    leftScore.setPosition(30, 10);
    rightScore.setPosition(670, 10);
    window.draw(leftScore);
    window.draw(rightScore);

    window.display();

    bool wait = true;
    while (wait && window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                wait = false;
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mouse(sf::Mouse::getPosition(window));
                if (button.getGlobalBounds().contains(mouse))
                    wait = false;
            }
        }
        sf::sleep(sf::milliseconds(10));
    }

    player1Score = 0;
    player2Score = 0;
    if (mode == GameMode::PvAI) aiLevel = 1;
    difficulty = Difficulty::Easy;
    state = GameState::Menu;
    saveScores();
}

sf::Vector2f Game::randomBallVelocity(float speed) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> angleDist(-60, 60);
    float angle = angleDist(rng);
    float radians = angle * 3.14159265f / 180.f;
    float vx = speed * ((rng() % 2 == 0) ? 1 : -1) * std::cos(radians);
    float vy = speed * std::sin(radians);
    return sf::Vector2f(vx, vy);
}