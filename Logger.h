//
// Created by vrenclouff on 26/11/16.
//


#ifndef NIMSERVER_LOGGER_H
#define NIMSERVER_LOGGER_H


#include <string>
#include <mutex>
#include <iostream>
#include <fstream>
#include <ios>

class Logger
{
private:
    mutable std::mutex m;

    int level;
    bool console;
    bool file;

    std::ofstream fs;

    void log(int const level, std::string const &prefix, std::string const &msg);


public:
    static Logger *instance();
    static void registerLogger(int const level, bool const console, bool const file);

    void debug(std::string const &msg);
    void info(std::string const &msg);
    void warning(std::string const &msg);
    void error(std::string const &msg);

};


#endif //NIMSERVER_LOGGER_H
