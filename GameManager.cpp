//
// Created by vrenclouff on 05/12/16.
//

#include "GameManager.h"


void GameManager::challenger(int const socket, std::vector<std::string> parameters)
{
    User &user_challenger = findUserBySocket(socket);

    if (NULL != (&user_challenger) && parameters.size() == 1)
    {
        User &user_recipient = findUserByLoginname(parameters[0]);

        if (NULL != (&user_recipient) && user_recipient.state == enums::LOGGED && user_challenger.state == enums::LOGGED)
        {
            logger->info(StringUtils::format(5, "User ", user_challenger.loginname.c_str(), " wants invite ", user_recipient.loginname.c_str()," to game."));
            user_challenger.state = enums::WAIT_FOR_GAME;
            user_recipient.state = enums::WAIT_FOR_GAME;
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

        if (NULL != (&user_challenger) && user_challenger.state == enums::WAIT_FOR_GAME)
        {
            if (result.compare("ACCEPT") == 0)
            {
                logger->info(StringUtils::format(5, "User ", user_recipient.loginname.c_str(), " accept invite from ", user_challenger.loginname.c_str(), " to game."));

                user_recipient.state = enums::PLAYED;
                user_challenger.state = enums::PLAYED;

                send_queue->push(SNDMessage(user_challenger.socket, enums::GAME_JOIN,
                                            StringUtils::format(5, "START", " ", std::to_string(matches_layers).c_str(), " ", std::to_string(matches_taking).c_str()).c_str()));
                send_queue->push(SNDMessage(user_recipient.socket, enums::GAME_JOIN,
                                            StringUtils::format(5, "STOP", " ", std::to_string(matches_layers).c_str(), " ", std::to_string(matches_taking).c_str()).c_str()));

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
        }
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
                        logger->info(StringUtils::format(4, "User with ID: ", std::to_string(user.socket).c_str(), " loses in game ", std::to_string(game.id).c_str()));
                        send_queue->push(SNDMessage(game.onTheTurn(),    enums::GAME_FINISH, "LOSE"));
                        send_queue->push(SNDMessage(game.onNotTheTurn(), enums::GAME_FINISH, "WIN"));

                        User &adversary_user = findUserBySocket(game.onNotTheTurn());

                        user.state = enums::LOGGED;
                        user.game = -1;

                        logger->info(StringUtils::format(3, "Game with ID: ", std::to_string(game.id).c_str(), " ended and will be deleted."));

                        adversary_user.state = enums::LOGGED;
                        adversary_user.game = -1;

                        int game_index = findGameIndex(game);
                        games->remove(game_index);

                        broadcast({user.socket, adversary_user.socket}, enums::LOGGED, enums::ALL_USERS);

                    }

                }else
                {
                    logger->info(StringUtils::format(4, "User with ID ", std::to_string(socket).c_str(), " can not take from layer ", std::to_string(layer).c_str()));
                    send_queue->push(SNDMessage(user.socket, enums::GAME_TAKE, ERROR));
                }

            }else
            {
                logger->info(StringUtils::format(3, "Player with ID ", std::to_string(socket).c_str(), " is not on turn."));
                send_queue->push(SNDMessage(user.socket, enums::GAME_TAKE, ERROR));
            }
        }
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
                }
            }
        }
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
        }
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
                User &game_user = findUserBySocket(game.player_first);
                if (NULL != (&game_user))
                {
                    send_queue->push(SNDMessage(game_user.socket, enums::GAME_END, SUCCESS));
                    game_user.state = enums::LOGGED;
                    game_user.game = -1;
                }
            }

            if (game.player_second != -1)
            {
                User &game_user = findUserBySocket(game.player_second);
                if (NULL != (&game_user))
                {
                    send_queue->push(SNDMessage(game_user.socket, enums::GAME_END, SUCCESS));
                    game_user.state = enums::LOGGED;
                    game_user.game = -1;
                }
            }

            logger->info(StringUtils::format(3, "Game ", std::to_string(game.id).c_str(), " was deleted."));
            int game_index = findGameIndex(game);
            games->remove(game_index);

            broadcast({socket}, enums::LOGGED, enums::ALL_USERS);
        }
        else
        {
            send_queue->push(SNDMessage(user.socket, enums::GAME_END, ERROR));
        }
    }else
    {
        send_queue->push(SNDMessage(socket, enums::GAME_END, ERROR));
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
                send_queue->push(SNDMessage(user.socket, enums::GAME_STATE, ("START " + state + " " + std::to_string(game.take_counter))));
            }else
            {
                send_queue->push(SNDMessage(user.socket, enums::GAME_STATE, ("STOP " + state + " 0")));
            }
        }
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
        send_queue->push(SNDMessage(socket, enums::GAME_SETTINGS, ERROR));
    }
}
