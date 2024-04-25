#include "rules.hpp"
#include <iostream>

Rules::Rules()
{
    if (!font.loadFromFile("Comic.ttf"))
    { // Ensure the path to the font is correct
        std::cerr << "Failed to load font\n";
    }

    rules_text.setFont(font);
    rules_text.setCharacterSize(19);
    rules_text.setFillColor(sf::Color::Green);
    rules_text.setString(
        "Rules:\n\n"
        "Enter the escape key to return back to main menu.\n"
        "Tic-tac-toe is a paper-and-pencil game for two players.\n"
        "where each player takes turns marking a 3x3 grid with X or O.\n"
        "The first player to get three of their marks in a row (up, down, or diagonally) wins.\n"
        "The game resets when all nine squares are full.\n"
        "To win the game a player have to win 3 rounds in a 5 round time frame.\n"
        "if a player have not won 3 rounds after the fifth round it will be a draw.");

    // Center the text on the screen
    rules_text.setPosition(sf::Vector2f(50, 50)); // Adjust position as needed
    rules_text.setLineSpacing(2.0f);              // Adjust line spacing as needed

    // animation init..
    circles.setPrimitiveType(sf::PrimitiveType::Points);
    initialize_circles();
}

void Rules::handle_input(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::KeyPressed: // when a key is pressed
            switch (event.key.code)
            {
            case sf::Keyboard::Escape: // Escape key pressed
                // Close the rules window and return to the main menu
                is_open = false;
                break;
            case sf::Keyboard::Unknown:
                // Handle unknown key press, if needed
                std::cout << "unkown key pressed";
                break;
            default:
                // Optionally handle other keys or do nothing
                break;
            }
            break;
        case sf::Event::Closed: // Close button clicked
            is_open = false;
            break;
        default:
            break;
        }
    }
}

void Rules::draw(sf::RenderWindow &window)
{
    window.clear(sf::Color::Black);
    window.draw(rules_text);
    window.draw(circles);
    window.display();
}

void Rules::run(sf::RenderWindow &window)
{
    is_open = true;
    while (window.isOpen() && is_open)
    {
        handle_input(window);
        update();
        draw(window);
    }
}

void Rules::update()
{
    move_circles();
}

void Rules::initialize_circles()
{
    srand(time(nullptr));
    for (int i = 0; i < 200; ++i)
    {                                                              // Create 100 circles
        float x = static_cast<float>(rand() % 800);                // Random x-coordinate
        float y = static_cast<float>(rand() % 600);                // Random y-coordinate
        sf::Color color(rand() % 256, rand() % 256, rand() % 256); // Random color
        circles.append(sf::Vertex(sf::Vector2f(x, y), color));     // Add circle vertex
    }
}

void Rules::move_circles()
{
    for (std::size_t i = 0; i < circles.getVertexCount(); ++i)
    {
        float speed = static_cast<float>(rand() % 3) / 10.0f; // Random speed
        circles[i].position.x += speed;                       // Move circle horizontally
        if (circles[i].position.x > 800)
        {                              // If circle reaches right edge
            circles[i].position.x = 0; // Move circle to left edge
        }
    }
}