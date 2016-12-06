//
// Created by vrenclouff on 26/11/16.
//

#ifndef NIMSERVER_NETWORK_H
#define NIMSERVER_NETWORK_H

#define STX         02
#define ETX         03
#define CHECKSUM    138


#include "Logger.h"
#include "Thread.h"
#include "SafeQueue.cpp"
#include "RCVMessage.h"

class NetworkService: public Thread {

private:
    Logger *logger;
    SafeQueue<RCVMessage> *queue;
    int server_socket;
    fd_set socks;
    int port;

    void initSocket(int port);
    void startListeningLoop();
    bool validationMessage(std::string const &message, std::string &validated_message);

public:
    NetworkService(SafeQueue<RCVMessage> *queue, int const port)
    {
        this->queue = queue;
        this->port = port;
        this->logger = Logger::instance();
    }

    virtual void run();
};


#endif //NIMSERVER_NETWORK_H
