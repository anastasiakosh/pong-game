#ifndef PADDLE_HPP
#define PADDLE_HPP

#include <SFML/Graphics.hpp>

class Paddle {
public:
    Paddle(float startX, float startY);
    void move(float dy, float dt);
    void reset(float startX, float startY);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    void setSize(float w, float h);
    void setDY(float value) { dy = value; }
    float getDY() const { return dy; }
private:
    sf::RectangleShape paddleShape;
    float dy = 0;
};

#endif // PADDLE_HPP