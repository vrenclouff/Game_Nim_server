//
// Created by vrenclouff on 20/10/16.
//

#include <iostream>
#include "Parameters.h"

void Parameters::parse(int argc, char **argv)
{
    MainOptions mo(argc, argv);

    MainOptions::Option* portOpt = mo.getParamFromKey("-p");
    _port = portOpt ? atoi((*portOpt).second.c_str()) : DEFAULT_PORT;

    MainOptions::Option* logConsole = mo.getParamFromKey("-c");
    _log_to_console = logConsole ? true : DEFAULT_LOG_CONSOLE;

    MainOptions::Option* logFile = mo.getParamFromKey("-f");
    _log_to_file = logFile ? true : DEFAULT_LOG_FILE;

    MainOptions::Option* logLevel = mo.getParamFromKey("-l");
    _log_level = logLevel ? atoi((*logLevel).second.c_str()) : DEFAULT_LOG_LEVEL;

    MainOptions::Option* matchesLayers = mo.getParamFromKey("-m");
    _matches_layers = matchesLayers ? atoi((*matchesLayers).second.c_str()) : DEFAULT_MATCHES_LAYERS;

//    MainOptions::Option* matchesTaking = mo.getParamFromKey("-t");
    _matches_taking = /* matchesTaking ? atoi((*matchesTaking).second.c_str()) : */ DEFAULT_MATCHES_TAKING;

    MainOptions::Option* help = mo.getParamFromKey("-h");
    _help = help ? true : false;


    if (_log_level < MIN_LOG_LEVEL || _log_level > MAX_LOG_LEVEL)
    {
        std::cout << "Log level " << _log_level << " is not in range <0,3>"  << std::endl; exit(1);
    }

    if (_matches_layers < MIN_MATCHES_LAYERS || _matches_layers > MAX_MATCHES_LAYERS)
    {
        std::cout << "Number of layers " << _matches_layers << " is not in range <3,6>" << std::endl; exit(1);
    }
}

void Parameters::printHelp()
{
    std::cout  << "NAME" << std::endl;
    std::cout  << "\tnimServer - server nim game" << std::endl;
    std::cout  << "OPTIONS" << std::endl;
    std::cout  << "\t-p" << std::endl;
    std::cout  << "\t\tport for access to server" << std::endl;
    std::cout  << "\t\t\tdefault - 9999" << std::endl;

    std::cout  << "\t-c" << std::endl;
    std::cout  << "\t\tenable log to console" << std::endl;
    std::cout  << "\t\t\tdefault - true" << std::endl;

    std::cout  << "\t-f" << std::endl;
    std::cout  << "\t\tenable log to file" << std::endl;
    std::cout  << "\t\t\tdefault - false" << std::endl;

    std::cout  << "\t-l" << std::endl;
    std::cout  << "\t\tdefine log level" << std::endl;
    std::cout  << "\t\t\tdefault - 1" << std::endl;
    std::cout  << "\t\t\t 0 - DEBUG" << std::endl;
    std::cout  << "\t\t\t 1 - INFO" << std::endl;
    std::cout  << "\t\t\t 2 - WARNING" << std::endl;
    std::cout  << "\t\t\t 3 - ERROR" << std::endl;

    std::cout  << "\t-m" << std::endl;
    std::cout  << "\t\tnumber of layers, which will be show in game" << std::endl;
    std::cout  << "\t\t\tdefault - 4" << std::endl;
    std::cout  << "\t\t\t 3 - 9 matches" << std::endl;
    std::cout  << "\t\t\t 4 - 16 matches" << std::endl;
    std::cout  << "\t\t\t 5 - 25 matches" << std::endl;
    std::cout  << "\t\t\t 6 - 36 matches" << std::endl;

    std::cout  << "\t-h" << std::endl;
    std::cout  << "\t\tshow help" << std::endl;
}
