//
// Created by vrenclouff on 27/11/16.
//

#ifndef NIMSERVER_THREAD_H
#define NIMSERVER_THREAD_H

#include <thread>

class Thread {
public:
    virtual void run() = 0;
};


#endif //NIMSERVER_THREAD_H
