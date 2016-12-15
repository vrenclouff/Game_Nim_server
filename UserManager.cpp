//
// Created by vrenclouff on 05/12/16.
//

#include "UserManager.h"

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
        }else if (user.state == enums::LOGGED)
        {
            int user_index = findUserIndex(user);
            users->remove(user_index);
            logger->info(StringUtils::format(3, "User ", user.loginname.c_str(), " was removed."));

            broadcast({socket}, enums::LOGGED, enums::ALL_USERS);
        }
    }
}

void UserManager::all_users(int const socket, std::vector<std::string> parameters)
{
    User &user = findUserBySocket(socket);

    logger->debug("Use want all players, who does not play.");

    if (NULL == (&user))
    {
        logger->debug("User does not logged.");
        send_queue->push(SNDMessage(socket, enums::ALL_USERS, ERROR));
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
            ss << (std::string("\"") + user_temp.loginname + std::string("\""));
            free_users++;
        }
    }
    std::string params = StringUtils::format(5, "{\"count\":", std::to_string(free_users).c_str() ,",\"users\":[", ss.str().c_str(), "]}");
    send_queue->push(SNDMessage(user.socket, enums::ALL_USERS, params));
}

void UserManager::login(int const socket, std::vector<std::string> parameters)
{
    if (parameters.size() != 1)
    {
        send_queue->push(SNDMessage(socket, enums::LOGIN, ERROR));
        return;
    }

    std::string loginname = parameters[0];

    if (loginname.length() == 0)
    {
        logger->info(StringUtils::format(3, "Username for ID ", std::to_string(socket).c_str(), " cannot be empty."));
        send_queue->push(SNDMessage(socket, enums::LOGIN, ERROR));
        return;
    }

    User &user = findUserByLoginname(loginname);

    if (NULL == (&user))
    {
        users->add(User(socket, loginname));
        logger->info(StringUtils::format(4, "New user with name ", loginname.c_str(), " and ID ", std::to_string(socket).c_str()));
        send_queue->push(SNDMessage(socket, enums::LOGIN, SUCCESS));
        send_queue->push(SNDMessage(socket, enums::GAME_SETTINGS,
                                    StringUtils::format(5, "{\"layers\":",std::to_string(matches_layers).c_str(),",\"taking\":",std::to_string(matches_taking).c_str(),"}")));

        broadcast({socket}, enums::LOGGED, enums::ALL_USERS);

    } else if (user.state == enums::DISCONNECTED)
    {
        user.socket = socket;
        // TODO vyzva na zpet do hry
        send_queue->push(SNDMessage(socket, enums::LOGIN, "back to game"));

    }else
    {
        logger->info(StringUtils::format(3, "User with loginname ", loginname.c_str(), " already exist."));
        send_queue->push(SNDMessage(socket, enums::LOGIN, ERROR));
    }
}

void UserManager::logout(int const socket, std::vector<std::string> parameters)
{
    User &user = findUserBySocket(socket);
    if (NULL == (&user))
    {
        send_queue->push(SNDMessage(socket, enums::LOGOUT, ERROR)); return;
    }

    enums::user_state state = user.state;

    int user_index = findUserIndex(user);
    users->remove(user_index);
    logger->info(StringUtils::format(3, "User with ID: ", std::to_string(user.socket).c_str(), " was disconnected."));
    send_queue->push(SNDMessage(socket, enums::LOGOUT, SUCCESS));

    if (state == enums::LOGGED)
    {
        broadcast({socket}, enums::LOGGED, enums::ALL_USERS);
    }else if (state == enums::PLAYED)
    {
        // TODO najdi jeho hry a dej hracum zpravu o ukonceni hry
    }
}

