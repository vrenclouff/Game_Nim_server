//
// Created by vrenclouff on 20/10/16.
//

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
}
