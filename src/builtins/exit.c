#include "builtins.h"

int exit_it(char **args, size_t len)
{
    int number = 0;
    if (len > 2)
    {
        warnx("exit: too many arguments");
        return 1;
    }
    if (len == 2)
        number = atoi(args[2]);
    if (number == -1)
    {
        warnx("exit: wrong type of argument provided");
        return 1;
    }
    exit(number);
}
