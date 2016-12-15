//
// Created by vrenclouff on 20/10/16.
//


#ifndef NIMSERVER_PARAMETERS_H
#define NIMSERVER_PARAMETERS_H

#define DEFAULT_PORT            9999
#define DEFAULT_LOG_LEVEL       1
#define DEFAULT_LOG_CONSOLE     false
#define DEFAULT_LOG_FILE        false
#define DEFAULT_MATCHES_LAYERS  4
#define DEFAULT_MATCHES_TAKING  3


#include "MainOptions.h"


class Parameters {

    int _port;

    int  _matches_layers;
    int  _matches_taking;
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
    int  matchesLayers()  { return _matches_layers; }
    int  matchesTaking()   { return _matches_taking;  }
    bool logToFile()      { return _log_to_file;    }
    bool logToConsole()   { return _log_to_console; }
    int  logLevel()       { return _log_level;      }
};


#endif //NIMSERVER_PARAMETERS_H
