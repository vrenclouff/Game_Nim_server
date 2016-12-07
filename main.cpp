
#include "Application.h"


int main(int argc, char **argv)
{
    Application::instance->start(new Parameters(argc, argv)); return 0;
}