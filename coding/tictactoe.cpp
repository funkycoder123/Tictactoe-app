#include "tictactoe.hpp"
#include <iostream>
#include <cstdlib> // For std::rand() and std::srand()
#include <ctime>   // For std::time()

TicTacToe::TicTacToe(bool vs_comp) : turn(1), scoreX(0), scoreO(0), vs_comp(vs_comp), rounds_played(0), is_open(false)
{
    // Load loss texture from GIF
    if (!lossTexture.loadFromFile("loss_animation.gif"))
    {
        // Failed to load loss animation texture
        std::cerr << "Failed to load loss animation texture!" << std::endl;
    }

    // Set texture for the loss sprite
    lossSprite.setTexture(lossTexture);

    // Set initial position and other properties for the loss sprite
    lossSprite.setPosition(200, 90); // Adjust position as needed
    lossSprite.setScale(2.0f, 2.0f); // Adjust scale as needed

    if (!celebrationTexture.loadFromFile("giphy.gif"))
    {
        // Failed to load celebration animation texture
        std::cerr << "Failed to load celebration animation texture!" << std::endl;
    }

    // Set texture for the celebration sprite
    celebrationSprite.setTexture(celebrationTexture);

    // Set initial position and other properties for the celebration sprite
    celebrationSprite.setPosition(0, 0);    // Adjust position as needed
    celebrationSprite.setScale(2.0f, 2.0f); // Adjust scale as needed

    if (!winBuffer.loadFromFile("win_sound.wav"))
    {
        // Failed to load sound file
        std::cerr << "Failed to load win sound file!" << std::endl;
    }

    // Set sound buffer for the win sound
    winSound.setBuffer(winBuffer);

    // Load lose sound file into sound buffer
    if (!loseBuffer.loadFromFile("lose_sound.wav"))
    {
        // Failed to load sound file
        std::cerr << "Failed to load lose sound file!" << std::endl;
    }

    // Set sound buffer for the lose sound
    loseSound.setBuffer(loseBuffer);

    if (!clickBuffer.loadFromFile("click_sound.wav"))
    {
        // Failed to load sound file
        std::cerr << "Failed to load click sound file!" << std::endl;
    }

    // Set sound buffer for the click sound
    clickSound.setBuffer(clickBuffer);

    if (!xTexture.loadFromFile("xx.png") || !oTexture.loadFromFile("oo.png"))
    {
        std::cerr << "): failed to load texture\n";
    }
    if (vs_comp)
    {
        std::srand(std::time(nullptr));
    }
    x_sprite.setTexture(xTexture);
    o_sprite.setTexture(oTexture);
    float symbolWidth = 50;
    float symbolHeight = 71;
    x_sprite.setScale(symbolWidth / xTexture.getSize().x, symbolHeight / xTexture.getSize().y);
    o_sprite.setScale(symbolWidth / oTexture.getSize().x, symbolHeight / oTexture.getSize().y);
    grid_sprite.setTextureRect(sf::IntRect(0, 0, 300, 300));
    grid_sprite.setPosition(200, 100);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            grid[i][j] = 0;
        }
    }
    if (!font.loadFromFile("Chalkduster.ttf"))
    {
        std::cerr << "Failed to load font!\n";
    }

    round_text.setFont(font);
    round_text.setCharacterSize(24); // Adjust size as needed
    round_text.setFillColor(sf::Color::White);
    update_round_text();

    score_text.setFont(font);
    score_text.setCharacterSize(24);
    score_text.setFillColor(sf::Color::White);
    update_score_text();
}

void TicTacToe::handle_input(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed && turn == 1)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                clickSound.play();
                handle_player_input(window);
            }
        }
    }

    if (vs_comp && turn == 2)
    {
        handle_computer_input();
    }
}

void TicTacToe::handle_player_input(sf::RenderWindow &window)
{
    int cell_size = 100;
    // converting mouse coord to grid indices..
    int row = (sf::Mouse::getPosition(window).y - 100) / cell_size;
    int col = (sf::Mouse::getPosition(window).x - 200) / cell_size;

    // If the clicked cell is empty and within bounds..
    if (row >= 0 && row < 3 && col >= 0 && col < 3 && grid[row][col] == 0)
    {

        grid[row][col] = turn; // cell will be marked with current player's symbol

        if (turn == 1)
        {
            x_sprite.setPosition(col * cell_size + 200 + (cell_size - xTexture.getSize().x) / 2, row * cell_size + 100 + (cell_size - xTexture.getSize().y) / 2);
        }
        else
        {
            o_sprite.setPosition(col * cell_size + 200 + (cell_size - oTexture.getSize().x) / 2, row * cell_size + 100 + (cell_size - oTexture.getSize().y) / 2);
        }
        // check for win ..
        if (check_win(turn))
        {
            // increment score for the winning player..
            if (turn == 1)
                scoreX++;
            else
                scoreO++;

            // update score dis
            update_score_text();

            // reset grid for a new game
            reset_grid();
            // rounds_played++;
        }
        else if (is_grid_full()) // checking for draw...
        {

            reset_grid();
            // rounds_played++;
        }
        else
        {

            turn = (turn == 1) ? 2 : 1; // switch turn
            draw(window);               // Redraw the window to show the player's move
            sf::sleep(sf::seconds(1));  // Delay after player's move
        }
    }
}

void TicTacToe::handle_computer_input()
{

    // Check if the computer can win in the next move
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (grid[i][j] == 0)
            {
                // Try placing the computer's symbol in this position
                grid[i][j] = 2; // Assuming computer's symbol is 2
                if (check_win(2))
                {
                    // Computer can win in the next move, place the symbol here
                    o_sprite.setPosition(j * 100 + 200 + (100 - oTexture.getSize().x) / 2, i * 100 + 100 + (100 - oTexture.getSize().y) / 2);
                    scoreO++;
                    update_score_text();
                    reset_grid();

                    return;
                }
                else
                {
                    // Undo the move
                    grid[i][j] = 0;
                }
            }
        }
    }

    // Check if the player can win in the next move, and block them
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (grid[i][j] == 0)
            {
                // Try placing the player's symbol in this position
                grid[i][j] = 1; // Assuming player's symbol is 1
                if (check_win(1))
                {
                    // Player can win in the next move, block them by placing the symbol here
                    grid[i][j] = 2; // Place computer's symbol
                    o_sprite.setPosition(j * 100 + 200 + (100 - oTexture.getSize().x) / 2, i * 100 + 100 + (100 - oTexture.getSize().y) / 2);
                    turn = 1; // Switch to player's turn
                    return;
                }
                else
                {
                    // Undo the move
                    grid[i][j] = 0;
                }
            }
        }
    }

    if (grid[1][1] == 0)
    {
        // Center is available, place the symbol there
        grid[1][1] = 2;
        o_sprite.setPosition(300, 200); // Center position
    }
    else
    {
        // Choose a random empty cell
        int row, col;
        do
        {
            row = rand() % 3;
            col = rand() % 3;
        } while (grid[row][col] != 0);

        grid[row][col] = 2; // Place the symbol in the random empty cell
        o_sprite.setPosition(col * 100 + 200 + (100 - oTexture.getSize().x) / 2, row * 100 + 100 + (100 - oTexture.getSize().y) / 2);
    }

    // Check for win after the computer's move
    if (check_win(2))
    {
        scoreO++;
        update_score_text();

        reset_grid();

        // rounds_played++;
    }
    else if (is_grid_full())
    {
        reset_grid();
        // rounds_played++;
    }
    else
    {
        turn = 1; // Switch to player's turn
    }
}

void TicTacToe::update()
{
    // Implement update function
}

void TicTacToe::draw(sf::RenderWindow &window)
{
    window.clear(sf::Color::Black);

    // draw grid lines

    sf::RectangleShape line(sf::Vector2f(3, 300)); // Vertical line 300
    line.setFillColor(sf::Color::White);
    line.setPosition(300, 100); // 300
    window.draw(line);

    line.setPosition(400, 100); // 500
    window.draw(line);

    line.setSize(sf::Vector2f(300, 3)); // Horizontal line 420
    line.setPosition(200, 200);         // 200
    window.draw(line);

    line.setPosition(200, 300); // 200,300
    window.draw(line);

    // draw X and O on the grid...

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (grid[i][j] == 1)
            {
                x_sprite.setPosition(j * 100 + 200 + (100 - xTexture.getSize().x) / 2, i * 100 + 100 + (100 - xTexture.getSize().y) / 2);
                window.draw(x_sprite);
            }
            else if (grid[i][j] == 2)
            {
                o_sprite.setPosition(j * 100 + 200 + (100 - oTexture.getSize().x) / 2, i * 100 + 100 + (100 - oTexture.getSize().y) / 2);
                window.draw(o_sprite);
            }
        }
    }

    // draw score text
    window.draw(score_text); // modify later..
    window.draw(round_text);
    window.display();

    // draw round text
}

void TicTacToe::run(sf::RenderWindow &window)
{
    is_open = true;
    while (window.isOpen())
    {
        handle_input(window);
        update();

        if (scoreX >= 3 || scoreO >= 3 || rounds_played >= 5)
        {
            bool draw = rounds_played >= 5 && scoreX < 3 && scoreO < 3;
            handle_end_game(window, draw);
            return;
        }

        /*
                if (scoreX >= 3 || scoreO >= 3 || (rounds_played >= 5 && (scoreX < 3 || scoreO < 3)))
                {
                    handle_end_game(window, (rounds_played >= 5 && (scoreX < 3 || scoreO < 3)));
                    return;
                }*/

        draw(window);
    }
}

bool TicTacToe::check_win(int player)
{

    // it will check rows,col and diagonals for three symbols in a row...

    for (int i = 0; i < 3; ++i)
    {
        if (grid[i][0] == player && grid[i][1] == player && grid[i][2] == player)
            return true;
        if (grid[0][i] == player && grid[1][i] == player && grid[2][i] == player)
            return true;
    }
    if (grid[0][0] == player && grid[1][1] == player && grid[2][2] == player)
        return true;
    if (grid[0][2] == player && grid[1][1] == player && grid[2][0] == player)
        return true;

    return false;
}

bool TicTacToe::is_grid_full()
{
    // If any cell is empty, the grid is not full
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (grid[i][j] == 0)
                return false;
        }
    }

    return true; // grid is full...
}

void TicTacToe::reset_grid()
{
    // clear all cells in the grid to restart game
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            grid[i][j] = 0;
        }
    }
    rounds_played++;
    update_round_text();
    // Reset turn to player 1
    turn = 1;
}

void TicTacToe::update_score_text()
{
    // display scores for both players
    score_text.setString("Score\n Player: " + std::to_string(scoreX) + "  Computer: " + std::to_string(scoreO));
    // Position score text on the window

    score_text.setPosition(400 - score_text.getLocalBounds().width - 10, 10); // modify later..
}

void TicTacToe::update_round_text()
{

    int windowWidth = 800;

    round_text.setString("Round played :" + std::to_string(rounds_played));

    round_text.setPosition(windowWidth - round_text.getLocalBounds().width - 10, 20);
}

void TicTacToe::handle_end_game(sf::RenderWindow &window, bool draw)
{
    sf::Text end_message;
    end_message.setFont(font);
    end_message.setCharacterSize(30);
    end_message.setFillColor(sf::Color::Black);
    end_message.setPosition(sf::Vector2f(50, 70));

    if (draw) //|| (rounds_played >= 5 && (scoreX <= 2 || scoreO <= 2))
    {

        end_message.setString("It's a draw do better next time!");
    }
    else if (scoreX >= 3)
    {
        end_message.setString("WOW congrats ,You won!");
        winSound.play();

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                    return;
                }
            }

            window.clear();

            // Draw celebration animation
            window.draw(celebrationSprite);

            window.display();
            sf::sleep(sf::seconds(5)); // Adjust the duration as needed
            break;
        }
    }

    else if (scoreO >= 3)
    {
        end_message.setString("Sorry,but you lost better luck next try!");
        loseSound.play();
        // Loss animation
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                    return;
                }
            }

            window.clear();

            // Draw loss animation
            window.draw(lossSprite);

            window.display();
            sf::sleep(sf::seconds(5)); // Adjust the duration as needed
            break;
        }
    }

    sf::Text return_message;
    return_message.setFont(font);
    return_message.setCharacterSize(20);
    return_message.setFillColor(sf::Color::Black);
    return_message.setString("Press Enter key to return to the main menu");
    return_message.setPosition(140, 400);

    while (true)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return;
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Return)
                {
                    is_open = false;
                    return;
                }
            }
        }

        window.clear(sf::Color::White);
        window.draw(end_message);
        window.draw(return_message);
        window.display();
    }
}
