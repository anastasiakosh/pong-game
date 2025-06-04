#ifndef BALL_HPP
#define BALL_HPP

#include <SFML/Graphics.hpp>

class Ball {
public:
    Ball(float startX, float startY, float speed = 300.f);
    void move(float dt);
    void reset();
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    sf::Vector2f getVelocity() const;
    void setVelocity(float vx, float vy);
private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float speed;
};

#endif // BALL_HPP