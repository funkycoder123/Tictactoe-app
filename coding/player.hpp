#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "tictactoe.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>

class Player : public TicTacToe
{

private:
    bool isServer;
    sf::IpAddress serverAddress;
    unsigned short port;
    sf::TcpSocket socket;
    sf::TcpListener listener;
    int current_turn;
    TicTacToe game;

    void send_game_state();
    void receive_game_state();
    std::string gameStateToString();
    void stringToGameState(const std::string &stateStr);

public:
    Player(bool isServer, unsigned short port);
    ~Player();

    void run(sf::RenderWindow &window) override;
    void handle_input(sf::RenderWindow &window) override;
};

#endif
