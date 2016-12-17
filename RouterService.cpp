//
// Created by vrenclouff on 28/11/16.
//

#include "RouterService.h"

void RouterService::run()
{

    logger->debug("Initialization game manager.");
    game_manager = new GameManager(users, games,receive_queue, send_queue, matches_layers, matches_taking);

    logger->debug("Initialization user manager.");
    user_manager = new UserManager(users, games,receive_queue, send_queue, matches_layers, matches_taking);

    logger->debug("Starting router loop.");
    startRouterLoop();
}

void RouterService::startRouterLoop()
{
    while(1)
    {
        logger->debug("Waiting for new receive message.");
        RCVMessage message = receive_queue->pop();

        switch (message.state)
        {
            case enums::PONG:               { send_queue->push(SNDMessage(message.socket, message.state, ""));}   break;
            case enums::HARD_LOGOUT:        { user_manager->hard_logout(message.socket, message.parameters);  }   break;
            case enums::ALL_USERS:          { user_manager->all_users(message.socket, message.parameters);    }   break;
            case enums::LOGIN:              { user_manager->login(message.socket, message.parameters);        }   break;
            case enums::LOGOUT:             { user_manager->logout(message.socket, message.parameters);       }   break;
            case enums::GAME_CHALLENGER:    { game_manager->challenger(message.socket, message.parameters);   }   break;
            case enums::GAME_INVITE:        { game_manager->invite(message.socket, message.parameters);       }   break;
            case enums::GAME_TAKE:          { game_manager->take(message.socket, message.parameters);         }   break;
            case enums::GAME_SWITCH_USER:   { game_manager->switch_user(message.socket, message.parameters);  }   break;
            case enums::GAME_BACK:          { game_manager->back(message.socket, message.parameters);         }   break;
            case enums::GAME_END:           { game_manager->end(message.socket, message.parameters);          }   break;
            case enums::GAME_STATE:         { game_manager->state(message.socket, message.parameters);        }   break;
            default:
            {
                send_queue->push(SNDMessage(message.socket, message.state, "success"));
            }
        }
    }
}


