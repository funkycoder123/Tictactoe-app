
#include "mainmenu.hpp"
#include <SFML/Graphics.hpp>

// Welcome
// BY lucky and David....

// main terminal......
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tic Tac Toe Main Menu");

    // object
    MainMenu main_menu;
    while (window.isOpen())
    {

        main_menu.handle_input(window);
        main_menu.update();
        window.clear(sf::Color::Black);
        main_menu.draw(window);
        window.display();
    }

    return 0;
}