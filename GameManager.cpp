//
// Created by vrenclouff on 05/12/16.
//

#include "GameManager.h"


void GameManager::challenger(int const socket, std::vector<std::string> parameters)
{
    User &user_challenger = findUserBySocket(socket);

    if (NULL != (&user_challenger) && parameters.size() != 0)
    {
        User &user_recipient = findUserByLoginname(parameters[0]);

        if (NULL != (&user_recipient) && user_recipient.state == enums::LOGGED && user_challenger.state == enums::LOGGED)
        {
            logger->info(StringUtils::format(5, "User ", user_challenger.loginname.c_str(), " wants invite ", user_recipient.loginname.c_str()," to game."));
            user_challenger.state = enums::WAIT_FOR_GAME;
            user_recipient.state = enums::WAIT_FOR_GAME;
            send_queue->push(SNDMessage(user_recipient.socket, enums::GAME_INVITE, user_challenger.loginname));
            broadcast({user_challenger.socket, user_recipient.socket}, enums::LOGGED, enums::ALL_USERS);
        }else
        {
            send_queue->push(SNDMessage(socket, enums::GAME_CHALLENGER, ERROR + SPACE + std::string("USER_RECIPIENT_NOT_FOUND")));
        }
    }else
    {
        send_queue->push(SNDMessage(socket, enums::GAME_CHALLENGER, ERROR + SPACE + std::string("USER_NOT_LOGGED")));
    }
}

void GameManager::invite(int const socket, std::vector<std::string> parameters)
{
    User &user_recipient = findUserBySocket(socket);

    if (NULL != (&user_recipient) && parameters.size() == 2)
    {
        std::string result = parameters[0];
        User &user_challenger = findUserByLoginname(parameters[1]);

        if (NULL != (&user_challenger) && user_challenger.state == enums::WAIT_FOR_GAME)
        {
            if (result.compare("ACCEPT") == 0)
            {
                logger->info(StringUtils::format(5, "User ", user_recipient.loginname.c_str(), " accept invite from ", user_challenger.loginname.c_str(), " to game."));

                user_recipient.state = enums::PLAYED;
                user_challenger.state = enums::PLAYED;

                send_queue->push(SNDMessage(user_challenger.socket,
                                            enums::GAME_JOIN, std::string("START") + SPACE + std::to_string(matches_layers) + SPACE + std::to_string(matches_taking)));
                send_queue->push(SNDMessage(user_recipient.socket,
                                            enums::GAME_JOIN, std::string("STOP") + SPACE + std::to_string(matches_layers) + SPACE + std::to_string(matches_taking)));

                game_reference_number++;
                logger->info(StringUtils::format(2, "Start game with ID: ", std::to_string(game_reference_number).c_str()));
                games->add(Game(user_challenger.socket, user_recipient.socket, matches_layers, matches_taking, game_reference_number));

                user_recipient.game = game_reference_number;
                user_challenger.game = game_reference_number;

            }else if (result.compare("IGNORE") == 0)
            {
                logger->info(StringUtils::format(5, "User ", user_recipient.loginname.c_str(), " ignore invite from ", user_challenger.loginname.c_str(), " to game."));

                user_recipient.state = enums::LOGGED;
                user_challenger.state = enums::LOGGED;

                broadcast({-1}, enums::LOGGED, enums::ALL_USERS);
            }
        }else
        {
            send_queue->push(SNDMessage(socket, enums::GAME_INVITE, ERROR + SPACE + std::string("USER_CHALLENGER_NOT_FOUND")));
        }
    }else
    {
        send_queue->push(SNDMessage(socket, enums::GAME_INVITE, ERROR + SPACE + std::string("USER_NOT_LOGGED")));
    }
}

void GameManager::take(int const socket, std::vector<std::string> parameters)
{
    User &user = findUserBySocket(socket);
    Game &game = findGameBySocket(socket);

    if (NULL != (&user) && NULL != (&game) && parameters.size() != 0)
    {
        if (user.state == enums::PLAYED)
        {
            if (game.onTheTurn() == user.socket)
            {
                int layer = std::atoi(parameters[0].c_str());

                if (game.take_counter > 0 && game.actualMatchesOnLayer(layer) > 0)
                {
                    logger->info(StringUtils::format(4, "User with ID: ", std::to_string(user.socket).c_str(), " takes on layer ", std::to_string(layer).c_str()));

                    game.takeOnLayer(layer);
                    game.take_counter--;

                    send_queue->push(SNDMessage(game.onTheTurn(),    enums::GAME_TAKE, StringUtils::format(2, "START ", std::to_string(layer).c_str())));
                    send_queue->push(SNDMessage(game.onNotTheTurn(), enums::GAME_TAKE, StringUtils::format(2, "STOP ", std::to_string(layer).c_str())));

                    if (game.matches() == 0)
                    {
                        receive_queue->push(RCVMessage(game.onNotTheTurn(), EnumUtils::to_string(enums::GAME_FINISH)));
                    }

                }else
                {
                    logger->info(StringUtils::format(4, "User with ID ", std::to_string(socket).c_str(), " can not take from layer ", std::to_string(layer).c_str()));
                    send_queue->push(SNDMessage(user.socket, enums::GAME_TAKE, ERROR + SPACE + std::string("CAN_NOT_TAKE")));
                }

            }else
            {
                logger->info(StringUtils::format(3, "Player with ID ", std::to_string(socket).c_str(), " is not on turn."));
                send_queue->push(SNDMessage(user.socket, enums::GAME_TAKE, ERROR + SPACE + std::string("NOT_ON_TURN")));
            }
        }else
        {
            send_queue->push(SNDMessage(socket, enums::GAME_TAKE, ERROR + SPACE + std::string("USER_DOES_NOT_PLAY")));
        }
    }else
    {
        send_queue->push(SNDMessage(socket, enums::GAME_TAKE, ERROR + SPACE + std::string("USER_NOT_LOGGED")));
    }
}

void GameManager::switch_user(int const socket, std::vector<std::string> parameters)
{
    User &user = findUserBySocket(socket);

    if (NULL != (&user))
    {
        if (user.state == enums::PLAYED)
        {
            Game &game = findGameByID(user.game);

            if (NULL != (&game))
            {
                if (game.onTheTurn() == user.socket)
                {
                    logger->info(StringUtils::format(2, "Switch user in game ", std::to_string(game.id).c_str()));

                    game.switchUser();
                    game.resetTake();

                    send_queue->push(SNDMessage(game.onTheTurn(),    enums::GAME_SWITCH_USER, "START"));
                    send_queue->push(SNDMessage(game.onNotTheTurn(), enums::GAME_SWITCH_USER, "STOP"));
                }else
                {
                    send_queue->push(SNDMessage(socket, enums::GAME_SWITCH_USER, ERROR + SPACE + std::string("NOT_ON_TURN")));
                }
            }else
            {
                send_queue->push(SNDMessage(socket, enums::GAME_SWITCH_USER, ERROR + SPACE + std::string("GAME_NOT_EXIST")));
            }
        }else
        {
            send_queue->push(SNDMessage(socket, enums::GAME_SWITCH_USER, ERROR + SPACE + std::string("USER_DOES_NOT_PLAY")));
        }
    }else
    {
        send_queue->push(SNDMessage(socket, enums::GAME_SWITCH_USER, ERROR + SPACE + std::string("USER_NOT_LOGGED")));
    }
}

void GameManager::back(int const socket, std::vector<std::string> parameters)
{
    User &user = findUserBySocket(socket);

    if (NULL != (&user) && parameters.size() != 0)
    {
        if (user.state == enums::DISCONNECTED)
        {
            std::string result = parameters[0];

            Game &game = findGameByID(user.game);

            if (NULL != (&game))
            {
                int adversary_socket;
                if (game.player_first == -1)
                {
                    adversary_socket = game.player_second;
                    game.player_first = user.socket;
                }else if (game.player_second == -1)
                {
                    adversary_socket = game.player_first;
                    game.player_second = user.socket;
                }

                if (result.compare("ACCEPT") == 0)
                {
                    logger->info(StringUtils::format(7, "Player ", std::to_string(user.socket).c_str()," wants come back to game ",
                                                     std::to_string(game.id).c_str()," and second player ", std::to_string(adversary_socket).c_str()," is waiting for him."));

                    user.state = enums::PLAYED;
                    receive_queue->push(RCVMessage(user.socket, EnumUtils::network_state_str[enums::GAME_STATE]));

                    User &adversary_user = findUserBySocket(adversary_socket);
                    adversary_user.state = enums::PLAYED;

                    if (adversary_socket == game.onTheTurn())
                    {
                        send_queue->push(SNDMessage(adversary_socket, enums::GAME_CONTINUE, ("START START")));
                    }else
                    {
                        send_queue->push(SNDMessage(adversary_socket, enums::GAME_CONTINUE, "START STOP"));
                    }

                }else if (result.compare("IGNORE") == 0)
                {
                    logger->info(StringUtils::format(7, "Player ", std::to_string(user.socket).c_str()," doesn't want come back to game ",
                                                     std::to_string(game.id).c_str()," and second player ", std::to_string(adversary_socket).c_str()," is waiting for him."));                    // TODO druha hrac ceka na pripojeni, ale on odmitl se prihlasit znovu

                    user.state = enums::LOGGED;
                    user.game = -1;
                    broadcast({-1}, enums::LOGGED, enums::ALL_USERS);

                    receive_queue->push(RCVMessage(adversary_socket, EnumUtils::network_state_str[enums::GAME_END]));
                }
            }else
            {
                logger->info(StringUtils::format(4, "Game with ID: ", std::to_string(user.game).c_str(), " does not exist. Second player ended game."));
                user.state = enums::LOGGED;
                broadcast({socket}, enums::LOGGED, enums::ALL_USERS);
            }
        }else
        {
            send_queue->push(SNDMessage(socket, enums::GAME_BACK, ERROR + SPACE + std::string("USER_IS_NOT_DISCONNECTED")));
        }
    }else
    {
        send_queue->push(SNDMessage(socket, enums::GAME_BACK, ERROR + SPACE + std::string("USER_NOT_LOGGED")));
    }
}

void GameManager::end(int const socket, std::vector<std::string> parameters)
{
    User &user = findUserBySocket(socket);

    if (NULL != (&user))
    {
        Game &game = findGameByID(user.game);

        if (NULL != (&game))
        {
            if (game.player_first != -1)
            {
                User &first_player = findUserBySocket(game.player_first);
                if (NULL != (&first_player))
                {
                    send_queue->push(SNDMessage(first_player.socket, enums::GAME_END, SUCCESS));
                    first_player.state = enums::LOGGED;
                    first_player.game = -1;
                }
            }

            if (game.player_second != -1)
            {
                User &second_player = findUserBySocket(game.player_second);
                if (NULL != (&second_player))
                {
                    send_queue->push(SNDMessage(second_player.socket, enums::GAME_END, SUCCESS));
                    second_player.state = enums::LOGGED;
                    second_player.game = -1;
                }
            }

            logger->info(StringUtils::format(3, "Game ", std::to_string(game.id).c_str(), " was deleted."));
            int game_index = findGameIndex(game);
            games->remove(game_index);

            broadcast({socket}, enums::LOGGED, enums::ALL_USERS);
        }
        else
        {
            send_queue->push(SNDMessage(socket, enums::GAME_END, ERROR + SPACE + std::string("GAME_NOT_EXIST")));
        }
    }else
    {
        send_queue->push(SNDMessage(socket, enums::GAME_END, ERROR + SPACE + std::string("USER_NOT_LOGGED")));
    }
}

void GameManager::state(int const socket, std::vector<std::string> parameters)
{
    User &user = findUserBySocket(socket);

    if (NULL != (&user))
    {
        Game &game = findGameByID(user.game);

        if (NULL != (&game))
        {
            std::string state = game.state();

            if (user.socket == game.onTheTurn())
            {
                send_queue->push(SNDMessage(user.socket, enums::GAME_STATE, ("START " + state + SPACE + std::to_string(game.take_counter))));
            }else
            {
                send_queue->push(SNDMessage(user.socket, enums::GAME_STATE, ("STOP " + state + " 0")));
            }
        }else
        {
            send_queue->push(SNDMessage(socket, enums::GAME_STATE, ERROR + SPACE + std::string("GAME_NOT_EXIST")));
        }
    }else
    {
        send_queue->push(SNDMessage(socket, enums::GAME_STATE, ERROR + SPACE + std::string("USER_NOT_LOGGED")));
    }
}

void GameManager::settings(int const socket, std::vector<std::string> parameters)
{
    User &user = findUserBySocket(socket);

    if (NULL != (&user))
    {
        send_queue->push(SNDMessage(socket, enums::GAME_SETTINGS,
                                    StringUtils::format(5, "{\"layers\":",std::to_string(matches_layers).c_str(),
                                                        ",\"taking\":",std::to_string(matches_taking).c_str(),"}")));
    }else
    {
        send_queue->push(SNDMessage(socket, enums::GAME_SETTINGS, ERROR + SPACE + std::string("USER_NOT_LOGGED")));
    }
}


void GameManager::finish(int const socket, std::vector<std::string> parameters)
{

    User &user_win = findUserBySocket(socket);

    if (NULL != (&user_win))
    {
        Game &game = findGameByID(user_win.game);

        if (NULL != (&game))
        {
            int adversary_socket = game.player_first == socket ? game.player_second : game.player_first;
            User &user_lose = findUserBySocket(adversary_socket);

            if (NULL != (&user_lose))
            {
                send_queue->push(SNDMessage(user_lose.socket, enums::GAME_FINISH, "LOSE"));
                send_queue->push(SNDMessage(user_win.socket, enums::GAME_FINISH, "WIN"));

                user_win.state = enums::LOGGED;
                user_win.game = -1;

                user_lose.state = enums::LOGGED;
                user_lose.game = -1;


                int game_index = findGameIndex(game);
                games->remove(game_index);

                broadcast({user_win.socket, user_lose.socket}, enums::LOGGED, enums::ALL_USERS);
            }
        }else
        {
            send_queue->push(SNDMessage(socket, enums::GAME_FINISH, ERROR + SPACE + std::string("GAME_NOT_EXIST")));
        }
    }else
    {
        send_queue->push(SNDMessage(socket, enums::GAME_FINISH, ERROR + SPACE + std::string("USER_NOT_LOGGED")));
    }
}