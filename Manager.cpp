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
            if (user.loginname.compare(loginname) == 0) { return user; }
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
    if (users->size() != 0)
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

Game &Manager::findGameBySocket(int const socket)
{
    if (games->size() != 0)
    {
        for (int i = 0; i < games->size(); i++)
        {
            Game &game = games->get(i);
            if (game.player_first == socket || game.player_second == socket)
            {
                return game;
            }
        }

    }
}

Game &Manager::findGameByID(int const id)
{
    if (games->size() != 0 || id != -1)
    {
        for (int i = 0; i < games->size(); i++)
        {
            Game &game = games->get(i);
            if (game.id == id)
            {
                return game;
            }
        }

    }
}

int Manager::findGameIndex(Game &game)
{
    if (games->size() != 0)
    {
        for (int i = 0; i < games->size(); i++)
        {
            Game &item = games->get(i);
            if (item.id == game.id) { return i; }
        }
    }
    return -1;
}
