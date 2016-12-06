//
// Created by vrenclouff on 05/12/16.
//

#ifndef NIMSERVER_USERMANAGER_H
#define NIMSERVER_USERMANAGER_H

#include "SafeList.cpp"
#include "SafeQueue.cpp"
#include "User.h"
#include "Game.h"
#include "SNDMessage.h"

class UserManager {
private:
    Logger *logger;
    SafeList<User> *users;
    SafeList<Game> *games;
    SafeQueue<SNDMessage> *send_queue;

public:
    UserManager(SafeList<User> *users, SafeList<Game> *games, SafeQueue<SNDMessage> *send_queue)
    {
        this->users = users;
        this->games = games;
        this->send_queue = send_queue;
        this->logger = Logger::instance();
    }
    User &findUserBySocket(int socket);
    User &findUserByLoginname(std::string const &loginname);
    int findUserIndex(User &user);

    void hard_logout(int const socket, std::vector<std::string> parameters);
    void all_users(int const socket, std::vector<std::string> parameters);
    void login(int const socket, std::vector<std::string> parameters);
    void logout(int const socket, std::vector<std::string> parameters);
};


#endif //NIMSERVER_USERMANAGER_H
