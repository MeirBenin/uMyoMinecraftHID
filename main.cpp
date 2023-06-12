#include "controller.h"

int main()
{
    Controller controller = Controller();


    while(!controller.initIMU());
    controller.run();
    return 0;
}