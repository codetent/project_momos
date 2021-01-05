#include "logic.h"

#include <unistd.h>


int main(int argc, char *argv)
{
    logic_init();

    while (1U)
    {
        logic_run();
        sleep(1);
    }

    return 0;
}
