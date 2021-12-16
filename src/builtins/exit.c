#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"

int is_digit(char *str)
{
    if (*str == '-' && *(str + 1) != '\0')
        str++;
    while (*str != '\0')
    {
        if (*str >= '0' && *str <= '9')
        {
            str++;
        }
        else
            return 0;
    }
    return 1;
}

int exit_it(char **args, size_t len)
{
    if (len == 1 || !is_digit(args[1]))
    {
        exit(0);
    }
    else
    {
        int n = atoi(args[1]);
        if (n >= 0 && n <= 255)
        {
            exit(n);
        }
        if (n < 0)
        {
            errx(2, "Illegal number: %d", n);
        }
        else
        {
            exit(0);
        }
    }
    return 0;
}