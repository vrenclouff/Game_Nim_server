//
// Created by vrenclouff on 29/11/16.
//

#ifndef NIMSERVER_SENDERSERVICE_H
#define NIMSERVER_SENDERSERVICE_H


#include "Thread.h"
#include "Logger.h"
#include "SafeQueue.cpp"
#include "SNDMessage.h"

class SenderService : public Thread {
private:
    Logger *logger;
    SafeQueue<SNDMessage> *send_queue;

    std::string createValidMessage(std::string const &msg);

public:
    SenderService(SafeQueue<SNDMessage> *send_queue)
    {
        this->send_queue = send_queue;
        this->logger = Logger::instance();
    }

    virtual void run();
};


#endif //NIMSERVER_SENDERSERVICE_H
