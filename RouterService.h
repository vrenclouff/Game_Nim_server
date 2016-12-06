//
// Created by vrenclouff on 28/11/16.
//

#ifndef NIMSERVER_GAMESERVICE_H
#define NIMSERVER_GAMESERVICE_H


#include "Thread.h"
#include "Logger.h"
#include "User.h"
#include "Game.h"
#include "RCVMessage.h"
#include "SNDMessage.h"
#include "GameManager.h"
#include "UserManager.h"

class RouterService : public Thread {
private:
    Logger *logger;
    GameManager *game_manager;
    UserManager *user_manager;

    SafeQueue<RCVMessage> *receive_queue;
    SafeQueue<SNDMessage> *send_queue;
    SafeList<User> *users;
    SafeList<Game> *games;

    int matches_count;
    int matches_choice;

    void startRouterLoop();

public:
    RouterService(SafeQueue<RCVMessage> *receive_queue, SafeQueue<SNDMessage> *send_queue, SafeList<User> *users, SafeList<Game> *games, int const matches_count, int const matches_choice)
    {
        this->receive_queue = receive_queue;
        this->send_queue = send_queue;
        this->users = users;
        this->games = games;
        this->logger = Logger::instance();
        this->matches_choice = matches_choice;
        this->matches_count = matches_count;
    }

    virtual void run();
};


#endif //NIMSERVER_GAMESERVICE_H
