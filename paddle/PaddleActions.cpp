#include "PaddleActions.hpp"
#include "Ball.hpp"
#include <cmath>

void handlePaddleMovement(Paddle& paddle, const sf::Keyboard::Key upKey, const sf::Keyboard::Key downKey, float paddleSpeed, float dt) {
    paddle.setDY(0);
    if (sf::Keyboard::isKeyPressed(upKey))
        paddle.setDY(-paddleSpeed);
    if (sf::Keyboard::isKeyPressed(downKey))
        paddle.setDY(paddleSpeed);
}

void resetPaddle(Paddle& paddle) {
    // Реализация сброса позиции, если нужно
}

void moveAI(Paddle& paddle, const Ball& ball, float dt) {
    float ballY = ball.getBounds().top + ball.getBounds().height / 2.f;
    float paddleY = paddle.getBounds().top + paddle.getBounds().height / 2.f;
    float aiSpeed = 220.f;

    if (std::abs(paddleY - ballY) > 10) {
        if (paddleY < ballY)
            paddle.setDY(aiSpeed);
        else
            paddle.setDY(-aiSpeed);
    } else {
        paddle.setDY(0);
    }
}