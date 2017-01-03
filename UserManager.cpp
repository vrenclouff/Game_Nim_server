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

        if (user.state == enums::PLAYED || user.state == enums::WAIT_FOR_GAME)
        {
            Game &game = findGameByID(user.game);

            if (NULL != (&game))
            {
                if (game.player_first == -1 || game.player_second == -1)
                {
                    logger->info(StringUtils::format(3, "Game ", std::to_string(game.id).c_str(), " does have players and will be deleted."));
                    int game_index = findGameIndex(game);
                    games->remove(game_index);

                    int user_index = findUserIndex(user);
                    users->remove(user_index);
                }else
                {
                    logger->info(StringUtils::format(3, "User ", user.loginname.c_str(), " was set state DISCONNECTED."));
                    user.state = enums::DISCONNECTED;

                    int adversary_socket;
                    if (game.player_first == socket)
                    {
                        adversary_socket = game.player_second;
                        game.player_first = -1;
                    } else if (game.player_second == socket)
                    {
                        adversary_socket = game.player_first;
                        game.player_second = -1;
                    }

                    send_queue->push(SNDMessage(adversary_socket, enums::GAME_DISCONNECT, ""));

                    User &adversary_user = findUserBySocket(adversary_socket);
                    adversary_user.state = enums::WAIT_FOR_GAME;
                }
            } else
            {
                int user_index = findUserIndex(user);
                users->remove(user_index);
            }
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

    if (NULL != (&user))
    {
        user.state = enums::LOGGED;
        std::stringstream ss;
        int free_users = 0;

        for (int i = 0; i < users->size(); i++) {
            User user_temp = users->get(i);
            if (user.socket == user_temp.socket) continue;
            if (user_temp.state == enums::LOGGED) {
                if (free_users != 0) { ss << ","; }
                ss << (std::string("\"") + user_temp.loginname + std::string("\""));
                free_users++;
            }
        }
        std::string params = StringUtils::format(5, "{\"count\":", std::to_string(free_users).c_str(), ",\"users\":[",  ss.str().c_str(), "]}");
        send_queue->push(SNDMessage(user.socket, enums::ALL_USERS, params));
    }else
    {
        send_queue->push(SNDMessage(socket, enums::ALL_USERS, ERROR + SPACE + std::string("USER_NOT_LOGGED")));
    }
}

void UserManager::login(int const socket, std::vector<std::string> parameters)
{
    if (parameters.size() != 0)
    {
        std::string username = parameters[0];

        if (username.length() != 0)
        {
            User &user = findUserByLoginname(username);

            if (NULL == (&user))
            {
                users->add(User(socket, username));
                logger->info(StringUtils::format(4, "New user with name ", username.c_str(), " and ID ", std::to_string(socket).c_str()));
                send_queue->push(SNDMessage(socket, enums::LOGIN, SUCCESS));
                broadcast({socket}, enums::LOGGED, enums::ALL_USERS);

            } else if (user.state == enums::DISCONNECTED || user.state == enums::WAIT_FOR_GAME)
            {
                user.socket = socket;

                Game &game = findGameByID(user.game);

                if (NULL != (&game))
                {
                    send_queue->push(SNDMessage(socket, enums::LOGIN, StringUtils::format(2, SUCCESS, " GAME")));

                } else
                {
                    send_queue->push(SNDMessage(socket, enums::LOGIN, SUCCESS));
                    user.state = enums::LOGGED;
                    broadcast({socket}, enums::LOGGED, enums::ALL_USERS);
                }
            } else
            {
                logger->info(StringUtils::format(3, "User with username ", username.c_str(), " already exist."));
                send_queue->push(SNDMessage(socket, enums::LOGIN, ERROR + SPACE + std::string("USER_EXIST")));
            }
        }else
        {
            send_queue->push(SNDMessage(socket, enums::LOGIN, ERROR + SPACE + std::string("USERNAME_IS_EMPTY")));
        }
    }else
    {
        send_queue->push(SNDMessage(socket, enums::LOGIN, ERROR + SPACE + std::string("USERNAME_MISSING")));
    }
}

void UserManager::logout(int const socket, std::vector<std::string> parameters)
{
    User &user = findUserBySocket(socket);

    if (NULL != (&user))
    {
        enums::user_state state = user.state;

        int user_index = findUserIndex(user);
        users->remove(user_index);

        logger->info(StringUtils::format(3, "User with ID: ", std::to_string(user.socket).c_str(), " was disconnected."));
        send_queue->push(SNDMessage(socket, enums::LOGOUT, SUCCESS));

        if (state == enums::LOGGED)
        {
            broadcast({socket}, enums::LOGGED, enums::ALL_USERS);

        } else if (state == enums::PLAYED)
        {
            Game &game = findGameByID(user.game);

            if (NULL != (&game))
            {
                int adversary_socket = game.player_first == socket ? game.player_second : game.player_first;
                receive_queue->push(RCVMessage(adversary_socket, EnumUtils::network_state_str[enums::GAME_END]));
            }
        }
    } else
    {
        send_queue->push(SNDMessage(socket, enums::LOGOUT, ERROR + SPACE + std::string("USER_NOT_EXIST")));
    }
}

