//
// Created by vrenclouff on 05/12/16.
//

#include "UserManager.h"
#include "StringUtils.h"

#include <sstream>
#include <iterator>

void UserManager::hard_logout(int const socket, std::vector<std::string> parameters)
{
    User &user = findUserBySocket(socket);

    if (NULL != (&user) || parameters.size() != 0)
    {
        logger->info(StringUtils::format(3, "User with ID: ", std::to_string(user.socket).c_str(), " was disconnected."));

        if (user.state == enums::PLAYED)
        {
            logger->info(StringUtils::format(3, "User ", user.loginname.c_str(), " was set state DISCONNECTED."));
            user.state = enums::DISCONNECTED;

            // TODO vyhledat hru a poslat protihracum se hrac se odpojil
        }else
        {
            int user_index = findUserIndex(user);
            users->remove(user_index);
            logger->info(StringUtils::format(3, "User ", user.loginname.c_str(), " was removed."));
        }
    }
}

void UserManager::all_users(int const socket, std::vector<std::string> parameters)
{
    User &user = findUserBySocket(socket);

    if (NULL == (&user) || parameters.size() != 0)
    {
        send_queue->push(SNDMessage(socket, enums::ALL_USERS, "ERROR"));
        return;
    }

    std::stringstream ss;
    int free_users = 0;

    for(int i=0; i<users->size(); i++)
    {
        User user_temp = users->get(i);
        if (user.socket == user_temp.socket) continue;
        if (user_temp.state == enums::LOGGED)
        {
            if(free_users != 0) { ss << ","; }
            ss << user_temp.loginname;
            free_users++;
        }
    }
    std::string params = StringUtils::format(5, "{count:", std::to_string(free_users).c_str() ,",users:[", ss.str().c_str(), "]}");
    send_queue->push(SNDMessage(user.socket, enums::ALL_USERS, params));
}

void UserManager::login(int const socket, std::vector<std::string> parameters)
{
    if (parameters.size() != 1)
    {
        send_queue->push(SNDMessage(socket, enums::LOGIN, "ERROR"));
        return;
    }

    std::string loginname = parameters[0];

    if (loginname.length() == 0)
    {
        logger->info(StringUtils::format(3, "Username for ID ", std::to_string(socket).c_str(), " cannot be empty."));
        send_queue->push(SNDMessage(socket, enums::LOGIN, "ERROR"));
        return;
    }

    User &user = findUserByLoginname(loginname);

    if (NULL == (&user))
    {
        users->add(User(socket, loginname));
        logger->info(StringUtils::format(4, "New user with name ", loginname.c_str(), " and ID ", std::to_string(socket).c_str()));
        send_queue->push(SNDMessage(socket, enums::LOGIN, "SUCCESS"));

    } else if (user.state == enums::DISCONNECTED)
    {
        user.socket = socket;
        // TODO vyzva na zpet do hry
        send_queue->push(SNDMessage(socket, enums::LOGIN, "back to game"));

    }else
    {
        logger->info(StringUtils::format(3, "User with loginname ", loginname.c_str(), " already exist."));
        send_queue->push(SNDMessage(socket, enums::LOGIN, "ERROR"));
    }
}

void UserManager::logout(int const socket, std::vector<std::string> parameters)
{
    User &user = findUserBySocket(socket);
    if (NULL == (&user) || parameters.size() != 0)
    {
        send_queue->push(SNDMessage(socket, enums::LOGOUT, "ERROR"));
        return;
    }

    // TODO najdi jeho hry a dej hracum zpravu o ukonceni hry

    int user_index = findUserIndex(user);
    users->remove(user_index);
    logger->info(StringUtils::format(3, "User with ID: ", std::to_string(user.socket).c_str(), " was disconnected."));
    send_queue->push(SNDMessage(socket, enums::LOGOUT, "SUCCESS"));
}

User &UserManager::findUserByLoginname(std::string const &loginname)
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

User &UserManager::findUserBySocket(int socket)
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

int UserManager::findUserIndex(User &user)
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