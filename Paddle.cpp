#include "Paddle.hpp"
#include <SFML/Graphics.hpp>

Paddle::Paddle(float startX, float startY) {
    paddleShape.setSize(sf::Vector2f(10, 100));
    paddleShape.setPosition(sf::Vector2f(startX, startY));
    paddleShape.setFillColor(sf::Color::White);
}

void Paddle::move(float dy, float dt) {
    sf::Vector2f pos = paddleShape.getPosition();
    float newY = pos.y + dy;
    if (newY < 0) newY = 0;
    if (newY > 600 - paddleShape.getSize().y) newY = 600 - paddleShape.getSize().y;
    paddleShape.setPosition(pos.x, newY);
}

void Paddle::reset(float startX, float startY) {
    paddleShape.setPosition(sf::Vector2f(startX, startY));
}

void Paddle::draw(sf::RenderWindow &window) {
    window.draw(paddleShape);
}

void Paddle::setSize(float w, float h) {
    paddleShape.setSize(sf::Vector2f(w, h));
    paddleShape.setOrigin(w / 2, h / 2);
}

sf::FloatRect Paddle::getBounds() const {
    return paddleShape.getGlobalBounds();
}