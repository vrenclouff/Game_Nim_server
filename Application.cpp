//
// Created by vrenclouff on 27/11/16.
//

#include "Application.h"
#include <algorithm>
#include <cstddef>

Application Application::instance()
{
    static Application instance;
    instance.logger = Logger::instance();
    return instance;
}

void Application::start(Parameters &param)
{

    Logger::registerLogger(param.logLevel(), param.logToConsole(), param.logToFile());
    logger->info("Start application.");

    logger->debug("Creating space for receive messages.");
    SafeQueue<RCVMessage> *receive_queue = new SafeQueue<RCVMessage>();

    logger->debug("Creating space for messages for send.");
    SafeQueue<SNDMessage> *send_queue = new SafeQueue<SNDMessage>();

    logger->debug("Creating space for users");
    SafeList<User> *users = new SafeList<User>();

    logger->debug("Creating space for games");
    SafeList<Game> *games = new SafeList<Game>();

    logger->debug("Creating network thread.");
    std::thread network_service(&Thread::run, NetworkService(receive_queue, param.port()));

    logger->debug("Creating router thread.");
    std::thread game_service(&Thread::run, RouterService(receive_queue, send_queue, users, games, param.matchesCount(), param.matchesChoice()));

    logger->debug("Creating sender thread.");
    std::thread sender_service(&Thread::run, SenderService(send_queue));

    network_service.join();
    game_service.join();
    sender_service.join();
}