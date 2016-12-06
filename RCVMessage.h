//
// Created by vrenclouff on 28/11/16.
//

#ifndef NIMSERVER_RCVMESSAGE_H
#define NIMSERVER_RCVMESSAGE_H

#include "StringUtils.h"
#include "Enums.h"


class RCVMessage {

private:
    void parse(std::string const &msg);

public:
    RCVMessage(int const socket, std::string const &msg)
    {
        this->socket = socket;
        parse(msg);
    }

    enums::network_state state;
    std::vector<std::string> parameters;
    int socket;
};

#endif //NIMSERVER_RCVMESSAGE_H
