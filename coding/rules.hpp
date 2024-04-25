
#ifndef RULES_H
#define RULES_H

#include "game.hpp"
#include <SFML/Graphics.hpp>

// rules class
class Rules : public Game
{
private:
    sf::Font font;
    sf::Text rules_text;
    bool is_open = false;
    sf::VertexArray circles; // array to store circles for animation..

public:
    Rules();

    void handle_input(sf::RenderWindow &window) override;
    void draw(sf::RenderWindow &window) override;
    void run(sf::RenderWindow &window);
    void update() override;
    void move_circles();
    void initialize_circles();

    ~Rules() = default; // destructor
};

#endif