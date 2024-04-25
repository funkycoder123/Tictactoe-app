#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "game.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib> // For std::rand() and std::srand()
#include <ctime>   // For std::time()
//  main game class all the functions for the game below

// inheritance
class TicTacToe : public Game
{

protected:
    int turn;
    void handle_player_input(sf::RenderWindow &window);
    // declaring design for x and o...
    sf::Texture xTexture;
    sf::Texture oTexture;
    sf::Sprite grid_sprite;
    sf::Sprite x_sprite;
    sf::Sprite o_sprite;
    int grid[3][3]; // the board is a 3*3 board....
    int scoreX;
    int scoreO;
    sf::Font font;
    sf::Text score_text;
    sf::Text round_text;
    bool vs_comp;
    int rounds_played;
    bool is_open;

    void handle_computer_input();
    bool check_win(int player);
    bool is_grid_full();
    void reset_grid();
    void update_score_text();
    void handle_end_game(sf::RenderWindow &window, bool draw);

    // sound effects
    sf::SoundBuffer clickBuffer;
    sf::Sound clickSound;

    sf::SoundBuffer winBuffer;
    sf::Sound winSound;

    sf::SoundBuffer loseBuffer;
    sf::Sound loseSound;

    // animation
    sf::Texture celebrationTexture;
    sf::Sprite celebrationSprite;
    sf::Texture lossTexture;
    sf::Sprite lossSprite;

public:
    // Constructor
    TicTacToe(bool vs_comp = false);
    void update() override;
    void draw(sf::RenderWindow &window) override;
    virtual void run(sf::RenderWindow &window);
    void handle_input(sf::RenderWindow &window) override;
    void update_round_text();

    friend class TicTacToeTest;
    friend void test_basic_win();

    ~TicTacToe() = default; // destructor
};

#endif