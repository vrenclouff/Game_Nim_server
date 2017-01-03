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

//    mutable std::mutex m;
//    std::condition_variable c;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

public:
    void push(T const&);
    T pop ();
};

template <class T>
void SafeQueue<T>::push (T const& elem)
{
    logger->debug("SafeQueue<>::push lock");
 //   std::lock_guard<std::mutex> lock(m);
    pthread_mutex_lock(&mutex);
    logger->debug("SafeQueue<>::push element");
    queue.push(elem);
    logger->debug("Broadcast condiditon call.");
 //   c.notify_one();
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

template <class T>
T SafeQueue<T>::pop ()
{
 //   std::unique_lock<std::mutex> lock(m);
    pthread_mutex_lock(&mutex);
    while(queue.empty())
    {
        logger->debug("SafeQueue<>::front wait.");
//        c.wait(lock);
        pthread_cond_wait(&cond, &mutex);
    }
    logger->debug("SafeQueue<>::front element.");
    T val = queue.front();
    logger->debug("SafeQueue<>::pop element");
    queue.pop();
    pthread_mutex_unlock(&mutex);
    return val;
}

#endif //NIMSERVER_SAFETYQUEUE_H