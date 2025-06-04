#include "Ball.hpp"

Ball::Ball(float startX, float startY, float speed_)
    : speed(speed_)
{
    shape.setRadius(10.f);
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(10.f, 10.f);
    shape.setPosition(startX, startY);
    reset();
}

void Ball::move(float dt) {
    shape.move(velocity * dt);
    sf::Vector2f pos = shape.getPosition();
    if (pos.y <= 0 || pos.y >= 600 - shape.getRadius() * 2)
        velocity.y = -velocity.y;
}

void Ball::reset() {
    shape.setPosition(400.f, 300.f);
    float angle = (rand() % 360) * 3.14f / 180.f;
    velocity = sf::Vector2f(speed * ((rand() % 2) ? 1.f : -1.f), speed * ((rand() % 2) ? 1.f : -1.f));
}

sf::Vector2f Ball::getVelocity() const { return velocity; }
void Ball::setVelocity(float vx, float vy) { velocity = {vx, vy}; }
sf::FloatRect Ball::getBounds() const { return shape.getGlobalBounds(); }
void Ball::draw(sf::RenderWindow& window) { window.draw(shape); }