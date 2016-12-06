//
// Created by vrenclouff on 26/11/16.
//

#include "Logger.h"
#include "StringUtils.h"

#include <ctime>
#include <iostream>
#include <locale>

Logger *Logger::instance()
{

    static Logger *instance = NULL;
    if (instance == NULL) {
        instance = new Logger();
    }
    return instance;
}

void Logger::registerLogger(int const level, bool const console, bool file)
{
    Logger *logger = Logger::instance();
    logger->level = level;
    logger->console = console;
    logger->file = file;
}

void Logger::log(int const aLevel, std::string const &prefix, std::string const &msg)
{
    if (aLevel >= level)
    {
        std::time_t t = std::time(NULL);
        char mbstr[20];
        std::strftime(mbstr, sizeof(mbstr), "%d %m %H:%M:%S %Y", std::localtime(&t));
        std::string log_message = StringUtils::format(5, &mbstr, " [", prefix.c_str(), "]\t", msg.c_str());
        if (console)
        {
            std::cout << log_message << std::endl;
        }
        if (file)
        {
            // TODO write to file
        }
    }
}

void Logger::debug(std::string const &msg)
{
    static const std::string prefix = "DEBUG";
    log(0, prefix, msg);
}

void Logger::info(std::string const &msg)
{
    static const std::string  prefix = "INFO";
    log(1, prefix, msg);
}

void Logger::warning(std::string const &msg)
{
    static const std::string  prefix = "WARNING";
    log(2, prefix, msg);
}

void Logger::error(std::string const &msg)
{
    static const std::string  prefix = "ERROR";
    log(3, prefix, msg);
}