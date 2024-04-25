#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "game.hpp"
#include "rules.hpp"
#include <SFML/Graphics.hpp>

// derived class
//  inheritance..
class MainMenu : public Game
{
private:
    int selected_item_index;
    sf::Font font;
    sf::Text menuItems[4]; // menu size
    Rules rules_screen;
    sf::VertexArray circles;

    void move_up();
    void move_down();

public:
    MainMenu();

    // member func
    void draw(sf::RenderWindow &window) override;
    void handle_input(sf::RenderWindow &window) override;
    void update() override;
    void run(sf::RenderWindow &window);
    int get_selectedItem_index() const;
    void move_circles();
    void initialize_circles();
    void handle_player_vs_player(sf::RenderWindow &MainMenuWindow);

    ~MainMenu() = default;
};

#endif