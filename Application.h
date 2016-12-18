//
// Created by vrenclouff on 27/11/16.
//

#ifndef NIMSERVER_APPLICATION_H
#define NIMSERVER_APPLICATION_H

#include "Parameters.h"
#include "NetworkService.h"
#include "RouterService.h"
#include "SenderService.h"

class Application {
private:
    Logger *logger;
    SafeQueue<RCVMessage> *receive_queue;
    SafeQueue<SNDMessage> *send_queue;
    SafeList<User> *users;
    SafeList<Game> *games;
public:
    ~Application();
    static Application *instance();
    void start(Parameters *param);
};


#endif //NIMSERVER_APPLICATION_H
