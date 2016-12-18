
#include "Application.h"
#include <signal.h>

void free_application(int sig)
{
    delete Application::instance();
    exit(0);
}

int main(int argc, char **argv)
{
    signal(SIGINT, free_application);
    Application::instance()->start(new Parameters(argc, argv)); return 0;
}