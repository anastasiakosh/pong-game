#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <SFML/Graphics.hpp>

class Background {
public:
    Background() = default;
    void draw(sf::RenderWindow& window);
};

#endif // BACKGROUND_HPP