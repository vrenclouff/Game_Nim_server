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

class GameManager {
private:
    SafeList<User> *users;
    SafeList<Game> *games;
    SafeQueue<SNDMessage> *send_queue;
public:
    GameManager(SafeList<User> *users, SafeList<Game> *games, SafeQueue<SNDMessage> *send_queue)
    {
        this->users = users;
        this->games = games;
        this->send_queue = send_queue;
    }
};


#endif //NIMSERVER_GAMEMANAGER_H
