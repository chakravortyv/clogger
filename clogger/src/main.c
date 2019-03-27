#include <stdio.h>

#include "incl/clogger.h"

void
another_function(void)
{
    clogger_log("err msg from another function", ERR);
}

int
main(void)
{
    /* The next two lines are just to make the debugger stop */
    //int debugI;
    //scanf("%d", &debugI);

    /* Initialize clogger's switches with values from `runtime.conf` */
    clogger_init();

    /* Call log messages and their severity from the application */
    clogger_log("off message", OFF);
    clogger_log("info message", INFO);
    clogger_log("err message", ERR);
    clogger_log("unk message", 99);
    clogger_log("fatal message", FATAL);
    clogger_log("info message", INFO);

    another_function();

    /* Shutdown clogger */
    clogger_terminate();

    return 0;
}

