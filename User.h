//
// Created by vrenclouff on 20/10/16.
//

#ifndef NIMSERVER_USER_H
#define NIMSERVER_USER_H

#include "Enums.h"
#include <string>


class User {

public:
    User(){}
    User(int socket, std::string const &loginname)
    {
        this->socket = socket;
        this->loginname = loginname;
        this->state = enums::LOGGED;
        this->game = -1;
    }

    int socket;
    std::string loginname;
    enums::user_state state;
    int game;
};


#endif //NIMSERVER_USER_H
