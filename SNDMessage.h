//
// Created by vrenclouff on 29/11/16.
//

#ifndef NIMSERVER_SNDMESSAGE_H
#define NIMSERVER_SNDMESSAGE_H

#include "Enums.h"
#include <string>

class SNDMessage {
public:
    SNDMessage(int const socket, enums::network_state const state, std::string const &parameters)
    {
        this->socket = socket;
        this->state = state;
        this->parameters = parameters;
    }
    enums::network_state state;
    std::string parameters;
    int socket;
};


#endif //NIMSERVER_SNDMESSAGE_H
