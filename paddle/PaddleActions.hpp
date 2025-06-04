#ifndef PADDLEACTIONS_HPP
#define PADDLEACTIONS_HPP

#include <SFML/Graphics.hpp>
#include "Paddle.hpp"
#include <SFML/Window/Keyboard.hpp>

void handlePaddleMovement(Paddle& paddle, const sf::Keyboard::Key upKey, const sf::Keyboard::Key downKey, float paddleSpeed, float dt);

// Function to reset paddle position
void resetPaddle(Paddle& paddle);

#endif // PADDLEACTIONS_HPP