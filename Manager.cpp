//
// Created by vrenclouff on 14/12/16.
//

#include "Manager.h"
#include <algorithm>

User &Manager::findUserByLoginname(std::string const &loginname)
{
    if (users->size() != 0)
    {
        for (int i = 0; i < users->size(); i++)
        {
            User &user = users->get(i);
            if (user.loginname == loginname) { return user; }
        }
    }
}

User &Manager::findUserBySocket(int const socket)
{
    if (users->size() != 0)
    {
        for (int i = 0; i < users->size(); i++)
        {
            User &user = users->get(i);
            if (user.socket == socket) { return user; }
        }
    }
}

int Manager::findUserIndex(User &user)
{
    {
        for (int i = 0; i < users->size(); i++)
        {
            User &item = users->get(i);
            if (item.socket == user.socket && item.loginname == user.loginname) { return i; }
        }
    }
    return -1;
}

void Manager::broadcast(std::vector<int> sockets, enums::user_state user_state, enums::network_state network_state)
{
    if (users->size() != 0)
    {
        for (int i = 0; i < users->size(); i++)
        {
            User user_temp = users->get(i);
            if (std::find(sockets.begin(), sockets.end(), user_temp.socket) != sockets.end()) continue;
            if (user_temp.state == user_state)
            {
                receive_queue->push(RCVMessage(user_temp.socket, EnumUtils::network_state_str[network_state]));
            }
        }
    }
}