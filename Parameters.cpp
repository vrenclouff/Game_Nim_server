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

    if (_port == 0)
    {
        std::cout << "Invalid port number" << std::endl; exit(1);
    }

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
    std::cout  << "JMÉNO" << std::endl;
    std::cout  << "\tnimServer - server hru Nim" << std::endl;
    std::cout  << "Možnosti" << std::endl;
    std::cout  << "\t-p" << std::endl;
    std::cout  << "\t\tčíslo portu" << std::endl;
    std::cout  << "\t\t\tvýchozí - 9999" << std::endl;

    std::cout  << "\t-c" << std::endl;
    std::cout  << "\t\tumožnit logování do konzole" << std::endl;
    std::cout  << "\t\t\tvýchozí - true" << std::endl;

    std::cout  << "\t-f" << std::endl;
    std::cout  << "\t\tumožnit logování do souboru (logfile.log)" << std::endl;
    std::cout  << "\t\t\tvýchozí - false" << std::endl;

    std::cout  << "\t-l" << std::endl;
    std::cout  << "\t\túroveň logování" << std::endl;
    std::cout  << "\t\t\tvýchozí - 1" << std::endl;
    std::cout  << "\t\t\t 0 - DEBUG" << std::endl;
    std::cout  << "\t\t\t 1 - INFO" << std::endl;
    std::cout  << "\t\t\t 2 - WARNING" << std::endl;
    std::cout  << "\t\t\t 3 - ERROR" << std::endl;

    std::cout  << "\t-m" << std::endl;
    std::cout  << "\t\tčíslo vrstev, které budou ve hře zobrazeny" << std::endl;
    std::cout  << "\t\t\tvýchozí - 4" << std::endl;
    std::cout  << "\t\t\t 3 - 9 sirek" << std::endl;
    std::cout  << "\t\t\t 4 - 16 sirek" << std::endl;
    std::cout  << "\t\t\t 5 - 25 sirek" << std::endl;
    std::cout  << "\t\t\t 6 - 36 sirek" << std::endl;

    std::cout  << "\t-h" << std::endl;
    std::cout  << "\t\tnápověda" << std::endl;
}
