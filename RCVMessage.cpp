//
// Created by vrenclouff on 28/11/16.
//

#include "RCVMessage.h"

void RCVMessage::parse(std::string const &msg)
{
    std::vector<std::string> array = StringUtils::split(msg, " ");
    std::string statusStr = array[0];
    array.erase(array.begin());

    this->parameters = array;
    this->state = EnumUtils::to_network_state(statusStr);
}