//
// Created by vrenclouff on 20/10/16.
//


#ifndef NIMSERVER_PARAMETERS_H
#define NIMSERVER_PARAMETERS_H

#define DEFAULT_PORT            9999
#define DEFAULT_LOG_LEVEL       1
#define DEFAULT_LOG_CONSOLE     false
#define DEFAULT_LOG_FILE        false

#include "MainOptions.h"


class Parameters {

    int _port;

    int  _matches_choice;
    int  _matches_count;
    bool _log_to_file;
    bool _log_to_console;
    int  _log_level;

private:
    void parse(int argc, char **argv);

public:
    Parameters(int argc, char **argv)
    {
        parse(argc, argv);
    }

    int  port()           { return _port;           }
    int  matchesChoice()  { return _matches_choice; }
    int  matchesCount()   { return _matches_count;  }
    bool logToFile()      { return _log_to_file;    }
    bool logToConsole()   { return _log_to_console; }
    int  logLevel()       { return _log_level;      }
};


#endif //NIMSERVER_PARAMETERS_H
