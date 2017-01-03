//
// Created by vrenclouff on 14/12/16.
//

#ifndef NIMSERVER_MANAGER_H
#define NIMSERVER_MANAGER_H

#include "User.h"
#include "Game.h"
#include "SafeList.cpp"
#include "SafeQueue.cpp"
#include "SNDMessage.h"
#include "RCVMessage.h"

#define SUCCESS "SUCCESS"
#define ERROR   "ERROR"
#define SPACE   std::string(" ")

class Manager {
protected:
    SafeList<User> *users;
    SafeList<Game> *games;
    SafeQueue<SNDMessage> *send_queue;
    SafeQueue<RCVMessage> *receive_queue;
    Logger *logger;

    int matches_layers;
    int matches_taking;
    int game_reference_number;

public:
    Manager(SafeList<User> *users, SafeList<Game> *games,SafeQueue<RCVMessage> *receive_queue,  SafeQueue<SNDMessage> *send_queue, int const matches_layers, int const matches_taking)
    {
        this->users = users;
        this->games = games;
        this->receive_queue = receive_queue;
        this->send_queue = send_queue;
        this->matches_layers = matches_layers;
        this->matches_taking = matches_taking;
        this->logger = Logger::instance();
        this->game_reference_number = 0;
    }

    User &findUserByLoginname(std::string const &loginname);
    User &findUserBySocket(int const socket);
    int  findUserIndex(User &user);

    Game &findGameBySocket(int const socket);
    Game &findGameByID(int const id);
    int  findGameIndex(Game &game);

    void broadcast(std::vector<int> sockets, enums::user_state user_state, enums::network_state network_state);

};

#endif //NIMSERVER_MANAGER_H
