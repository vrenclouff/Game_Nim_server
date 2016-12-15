//
// Created by vrenclouff on 05/12/16.
//

#ifndef NIMSERVER_USERMANAGER_H
#define NIMSERVER_USERMANAGER_H

#include "Manager.h"

class UserManager : public Manager {
public:
    UserManager(SafeList<User> *users, SafeList<Game> *games,SafeQueue<RCVMessage> *receive_queue,  SafeQueue<SNDMessage> *send_queue, int const matches_layers, int const matches_taking)
            : Manager(users, games, receive_queue, send_queue, matches_layers, matches_taking) {}

    void hard_logout(int const socket, std::vector<std::string> parameters);
    void all_users(int const socket, std::vector<std::string> parameters);
    void login(int const socket, std::vector<std::string> parameters);
    void logout(int const socket, std::vector<std::string> parameters);
};


#endif //NIMSERVER_USERMANAGER_H
