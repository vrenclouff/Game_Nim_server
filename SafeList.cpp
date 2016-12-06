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

    mutable std::mutex m;

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
    std::lock_guard<std::mutex> lock(m);
    logger->debug("SafeList<>::add element");
    array.push_back(elem);
}

template <class T>
void SafeList<T>::remove(int const index)
{
    logger->debug("SafeList<>::delete lock");
    std::lock_guard<std::mutex> lock(m);
    logger->debug("SafeList<>::delete element");
    array.erase(array.begin()+index);
}

#endif //NIMSERVER_SAFETYLIST_H