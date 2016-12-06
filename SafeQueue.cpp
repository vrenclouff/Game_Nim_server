//
// Created by vrenclouff on 27/11/16.
//


#ifndef NIMSERVER_SAFETYQUEUE_H
#define NIMSERVER_SAFETYQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include "Logger.h"

template <class T>
class SafeQueue {
private:
    Logger *logger = Logger::instance();
    std::queue<T> queue;

    mutable std::mutex m;
    std::condition_variable c;

public:
    void push(T const&);
    T pop ();
};

template <class T>
void SafeQueue<T>::push (T const& elem)
{
    logger->debug("SafeQueue<>::push lock");
    std::lock_guard<std::mutex> lock(m);
    logger->debug("SafeQueue<>::push element");
    queue.push(elem);
    logger->debug("Broadcast condiditon call.");
    c.notify_one();
}

template <class T>
T SafeQueue<T>::pop ()
{
    std::unique_lock<std::mutex> lock(m);
    while(queue.empty())
    {
        logger->debug("SafeQueue<>::front wait.");
        c.wait(lock);
    }
    logger->debug("SafeQueue<>::front element.");
    T val = queue.front();
    logger->debug("SafeQueue<>::pop element");
    queue.pop();
    return val;
}

#endif //NIMSERVER_SAFETYQUEUE_H