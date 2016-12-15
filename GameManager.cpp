//
// Created by vrenclouff on 05/12/16.
//

#include "GameManager.h"

void GameManager::join(int const socket, std::vector<std::string> parameters)
{


}

void GameManager::challenger(int const socket, std::vector<std::string> parameters)
{
    User &user_challenger = findUserBySocket(socket);

    if (NULL != (&user_challenger) && parameters.size() == 1)
    {
        User &user_recipient = findUserByLoginname(parameters[0]);

        if (NULL != (&user_recipient) && user_recipient.state == enums::LOGGED && user_challenger.state == enums::LOGGED)
        {
            logger->info(StringUtils::format(5, "User ", user_challenger.loginname.c_str(), " wants invite ", user_recipient.loginname.c_str()," to game."));
          //  user_challenger.state = enums::WAIT_FOR_GAME;
          //  user_recipient.state = enums::WAIT_FOR_GAME;
            send_queue->push(SNDMessage(user_recipient.socket, enums::GAME_INVITE, user_challenger.loginname));
            broadcast({user_challenger.socket, user_recipient.socket}, enums::LOGGED, enums::ALL_USERS);
        }
    }
}

void GameManager::invite(int const socket, std::vector<std::string> parameters)
{
    User &user_recipient = findUserBySocket(socket);

    if (NULL != (&user_recipient) && parameters.size() == 2)
    {
        std::string result = parameters[0];
        User &user_challenger = findUserByLoginname(parameters[1]);

        if (NULL != (&user_challenger))
        {
            if (result.compare("ACCEPT") == 0)
            {
                send_queue->push(SNDMessage(user_challenger.socket, enums::GAME_JOIN, "START"));
                send_queue->push(SNDMessage(user_recipient.socket, enums::GAME_JOIN, "STOP"));

            }else if (result.compare("IGNORE"))
            {

            }
        }
    }
}
