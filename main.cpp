
#include "Application.h"


int main(int argc, char **argv)
{
    Parameters params = Parameters(argc, argv);
    Application::instance().start(params); return 0;
}