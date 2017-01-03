//
// Created by vrenclouff on 05/12/16.
//

#ifndef NIMSERVER_GAMEMANAGER_H
#define NIMSERVER_GAMEMANAGER_H

#include "SafeList.cpp"
#include "SafeQueue.cpp"
#include "User.h"
#include "Game.h"
#include "SNDMessage.h"
#include "Manager.h"

class GameManager : public Manager {

public:
    GameManager(SafeList<User> *users, SafeList<Game> *games,SafeQueue<RCVMessage> *receive_queue,  SafeQueue<SNDMessage> *send_queue, int const matches_layers, int const matches_taking)
            : Manager(users, games, receive_queue, send_queue, matches_layers, matches_taking) {}

    void invite(int const socket, std::vector<std::string> parameters);
    void challenger(int const socket, std::vector<std::string> parameters);
    void take(int const socket, std::vector<std::string> parameters);
    void switch_user(int const socket, std::vector<std::string> parameters);
    void back(int const socket, std::vector<std::string> parameters);
    void end(int const socket, std::vector<std::string> parameters);
    void state(int const socket, std::vector<std::string> parameters);
    void settings(int const socket, std::vector<std::string> parameters);
    void finish(int const socket, std::vector<std::string> parameters);
};


#endif //NIMSERVER_GAMEMANAGER_H
