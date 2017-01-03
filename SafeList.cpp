//
// Created by vrenclouff on 04/12/16.
//

#ifndef NIMSERVER_SAFETYLIST_H
#define NIMSERVER_SAFETYLIST_H

#include <vector>
#include <mutex>
#include "Logger.h"

template <class T>
class SafeList {
private:
    Logger *logger = Logger::instance();
    std::vector<T> array;

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

public:
    void add(T const &);
    void remove(int const index);
    T &get(int const index);
    int size() { return array.size(); }
};


template <class T>
T &SafeList<T>::get(int const index)
{
    return array[index];
}

template <class T>
void SafeList<T>::add (T const & elem)
{
    logger->debug("SafeList<>::add lock");
    pthread_mutex_lock(&mutex);
    logger->debug("SafeList<>::add element");
    array.push_back(elem);
    pthread_mutex_unlock(&mutex);
}

template <class T>
void SafeList<T>::remove(int const index)
{
    logger->debug("SafeList<>::delete lock");
    pthread_mutex_lock(&mutex);
    logger->debug("SafeList<>::delete element");
    array.erase(array.begin()+index);
    pthread_mutex_unlock(&mutex);
}

#endif //NIMSERVER_SAFETYLIST_H