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
public:
    static Application instance();
    void start(Parameters &param);
};


#endif //NIMSERVER_APPLICATION_H
