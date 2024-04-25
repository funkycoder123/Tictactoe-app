#include "player.hpp"
#include <iostream>
#include <sstream>
#include <thread>
#include <stdexcept>

Player::Player(bool isServer, unsigned short port) : isServer(isServer), port(port), game(false)
{
    if (isServer)
    {
        sf::TcpListener listener;
        if (listener.listen(port) != sf::Socket::Done)
        {
            throw std::runtime_error("Server could not listen on port.");
        }
        std::cout << "Server ready and listening on port " << port << std::endl;
        sf::TcpSocket socket;
        if (listener.accept(socket) != sf::Socket::Done)
        {
            throw std::runtime_error("Server could not accept a connection.");
        }
        std::cout << "Client connected to server." << std::endl;
    }
    else
    {
        sf::TcpSocket socket;
        if (socket.connect(sf::IpAddress::getLocalAddress(), port) != sf::Socket::Done)
        {
            throw std::runtime_error("Client could not connect to server.");
        }
        std::cout << "Client connected to server." << std::endl;
    }
}

Player::~Player()
{
    socket.disconnect();
}

void Player::run(sf::RenderWindow &window)
{

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        game.handle_input(window);
        window.clear();
        game.draw(window);
        game.run(window);
    }
}

void Player::handle_input(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            // Call the base class method to handle player inputs based on the current turn
            if (turn == (isServer ? 1 : 2))
            {
                TicTacToe::handle_player_input(window);
                std::cout << "Sending game state..." << std::endl;
                send_game_state();
                std::cout << "Game state sent." << std::endl;
                current_turn = (current_turn == 1) ? 2 : 1; // switch ...
            }
        }
    }

    // Receive game state updates if it's not this player's turn
    if (current_turn != (isServer ? 1 : 2))
    {
        receive_game_state();
    }
}

void Player::send_game_state()
{
    std::string state = gameStateToString();
    if (socket.send(state.c_str(), state.size() + 1) != sf::Socket::Done)
    {
        std::cerr << "Failed to send game state." << std::endl;
    }
}

void Player::receive_game_state()
{
    char buffer[2000];
    std::size_t received;
    if (socket.receive(buffer, sizeof(buffer), received) == sf::Socket::Done)
    {
        buffer[received] = '\0';
        stringToGameState(std::string(buffer, received));
        current_turn = (isServer ? 1 : 2);
    }
}

std::string Player::gameStateToString()
{
    std::stringstream ss;
    // Serialize the grid
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ss << grid[i][j];
        }
    }

    ss << "|" << current_turn;
    ss << "|" << scoreX << "|" << scoreO;

    return ss.str();
}

void Player::stringToGameState(const std::string &stateStr)
{
    if (stateStr.length() < 11)
    { // Minimum length check
        std::cerr << "Error: Received game state string is too short." << std::endl;
        return;
    }

    std::istringstream iss(stateStr);
    char delimiter;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            iss >> grid[i][j];
        }
    }

    iss >> delimiter; // Read the delimiter
    if (delimiter != '|')
    {
        std::cerr << "Parsing error: Expected delimiter not found." << std::endl;
        return;
    }

    iss >> current_turn >> delimiter >> scoreX >> delimiter >> scoreO;
}