#include "mainmenu.hpp"
#include "tictactoe.hpp"
#include "player.hpp"
#include <iostream>
#include <thread>
#include <SFML/Graphics.hpp>
#include <mutex>
#include <condition_variable>

MainMenu::MainMenu() : selected_item_index(0)
{
    if (!font.loadFromFile("Chalkduster.ttf"))
    {
        std::cerr << "Failed to load font\n";
    }

    menuItems[0].setFont(font);
    menuItems[0].setString("Player vs Player");
    menuItems[0].setPosition(sf::Vector2f(100, 100));

    menuItems[1].setFont(font);
    menuItems[1].setString("Player vs Computer");
    menuItems[1].setPosition(sf::Vector2f(100, 150));

    menuItems[2].setFont(font);
    menuItems[2].setString("Rules");
    menuItems[2].setPosition(sf::Vector2f(100, 200));

    menuItems[3].setFont(font);
    menuItems[3].setString("Quit");
    menuItems[3].setPosition(sf::Vector2f(100, 250));

    for (int i = 0; i < 4; ++i)
    {
        menuItems[i].setFillColor(sf::Color::Green); // Default color
    }
    menuItems[selected_item_index].setFillColor(sf::Color::White); // Highlight the first item

    circles.setPrimitiveType(sf::PrimitiveType::Points);
    initialize_circles();
}

void MainMenu::draw(sf::RenderWindow &window)
{
    for (int i = 0; i < 4; ++i)
    {
        window.draw(menuItems[i]);
        window.draw(circles);
    }
}

void MainMenu::move_up()
{
    if (selected_item_index > 0)
    {
        menuItems[selected_item_index].setFillColor(sf::Color::Green);
        selected_item_index--;
        menuItems[selected_item_index].setFillColor(sf::Color::White);
    }
}

void MainMenu::move_down()
{
    if (selected_item_index < 3)
    {
        menuItems[selected_item_index].setFillColor(sf::Color::Green);
        selected_item_index++;
        menuItems[selected_item_index].setFillColor(sf::Color::White);
    }
}

int MainMenu::get_selectedItem_index() const
{
    return selected_item_index;
}

void MainMenu::run(sf::RenderWindow &window)
{
    while (window.isOpen())
    {
        handle_input(window);
        update();
        draw(window);
    }
}

void MainMenu::handle_input(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::KeyReleased: // when a key is released...
            switch (event.key.code)
            {
            case sf::Keyboard::Up:
                move_up();
                break;
            case sf::Keyboard::Down:
                move_down();
                break;
            case sf::Keyboard::Enter:
            {

                switch (get_selectedItem_index()) // was ==0
                {
                case 0: // Player vs Player
                    std::cout << "Player vs Player mode selected.\n";
                    // Launching multiplayer mode...
                    {
                        window.setVisible(false);
                        std::thread serverThread([]()
                                                 {
                        try {
                            sf::RenderWindow serverWindow(sf::VideoMode(800, 600), "Server Window");
                            Player serverPlayer(true, 54000);
                            serverPlayer.run(serverWindow);
                        } catch (const std::exception& e) {
                            std::cerr << "Server Thread Error: " << e.what() << std::endl;
                        } });

                        try
                        {
                            sf::RenderWindow clientWindow(sf::VideoMode(800, 600), "Client Window");
                            Player clientPlayer(false, 54000);
                            clientPlayer.run(clientWindow);
                        }
                        catch (const std::exception &e)
                        {
                            std::cerr << "Client Error: " << e.what() << std::endl;
                        }

                        serverThread.join(); // Wait for server thread to finish
                        window.setVisible(true);
                        // handle_player_vs_player(window);
                    }

                    break;
                case 1: // Player vs Computer
                    std::cout << "Player vs Computer mode selected.\n";
                    // Transition to TicTacToe game
                    {
                        TicTacToe TicTacToeGame(true);
                        TicTacToeGame.run(window);
                    }
                    break;
                case 2: // Rules
                    std::cout << "Rules printed on window.\n";
                    {
                        rules_screen.run(window);
                    }
                    break;
                case 3:             // Quit
                    window.close(); // Close render window
                    break;
                default:
                    break;
                }
            }
            break;
            }
            break;
        case sf::Event::Closed:
            window.close();
            break;
        }
    }
}

void MainMenu::update()
{
    move_circles();
}

void MainMenu::initialize_circles()
{
    srand(time(nullptr));
    for (int i = 0; i < 200; ++i)
    {                                                              // Create 200 circles
        float x = static_cast<float>(rand() % 800);                // Random x-coordinate
        float y = static_cast<float>(rand() % 600);                // Random y-coordinate
        sf::Color color(rand() % 256, rand() % 256, rand() % 256); // Random color
        circles.append(sf::Vertex(sf::Vector2f(x, y), color));     // Add circle vertex
    }
}

void MainMenu::move_circles()
{
    for (std::size_t i = 0; i < circles.getVertexCount(); ++i)
    {
        float speed = static_cast<float>(rand() % 5) / 10.0f; // Random speed
        circles[i].position.x += speed;                       // Move circle horizontally
        if (circles[i].position.x > 800)
        {                              // If circle reaches right edge
            circles[i].position.x = 0; // Move circle to left edge
        }
    }
}
