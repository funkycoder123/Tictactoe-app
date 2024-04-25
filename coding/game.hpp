// base class
#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

class Game
{
public:
    virtual void handle_input(sf::RenderWindow &window) = 0;
    virtual void update() = 0;
    virtual void draw(sf::RenderWindow &window) = 0;
};

#endif // GAME_HPP