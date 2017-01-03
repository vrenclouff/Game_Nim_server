//
// Created by vrenclouff on 27/11/16.
//

#include "Application.h"
#include <algorithm>
#include <cstddef>

Application *Application::instance()
{
    static Application *instance = new Application();
    instance->logger = Logger::instance();
    return instance;
}

void Application::start(Parameters *param)
{
    if (param->help()) { param->printHelp(); exit(0); }

    Logger::registerLogger(param->logLevel(), param->logToConsole(), param->logToFile());
    logger->info("Start application.");

    logger->debug("Creating space for receive messages.");
    receive_queue = new SafeQueue<RCVMessage>();

    logger->debug("Creating space for messages for send.");
    send_queue = new SafeQueue<SNDMessage>();

    logger->debug("Creating space for users");
    users = new SafeList<User>();

    logger->debug("Creating space for games");
    games = new SafeList<Game>();

    logger->debug("Creating network thread.");
    std::thread network_service(&Thread::run, NetworkService(receive_queue, send_queue, param->port()));

    logger->debug("Creating router thread.");
    std::thread router_service(&Thread::run, RouterService(receive_queue, send_queue, users, games, param->matchesLayers(), param->matchesTaking()));

    logger->debug("Creating sender thread.");
    std::thread sender_service(&Thread::run, SenderService(send_queue));

    network_service.join();
    router_service.join();
    sender_service.join();
}

Application::~Application()
{
    logger->info("Deleting space for receive messages.");
    delete receive_queue;

    logger->info("Deleting space for messages for send.");
    delete send_queue;

    logger->info("Deleting space for users.");
    delete users;

    logger->info("Deleting space for games.");
    delete games;

    logger->info("Application closed.");
    delete logger;
}
