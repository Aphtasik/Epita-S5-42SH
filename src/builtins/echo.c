#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_it(char *str, int e)
{
    if (!str)
        return;
    size_t i = 0;
    while (str[i])
    {
        if (str[i] == '\\')
        {
            if (e)
            {
                if (str[i + 1] != 0)
                {
                    if (str[i + 1] == 't')
                    {
                        printf("%c", '\t');
                        i++;
                    }
                    else if (str[i + 1] == 'n')
                    {
                        printf("%c", '\n');
                        i++;
                    }
                    else
                    {
                        printf("%c", '\\');
                    }
                }
            }
            else
            {
                i++;
                continue;
            }
        }
        else
            printf("%c", str[i]);
        i++;
    }
}

int echo_it(char **args, size_t len)
{
    int e = 0;
    int n = 0;
    size_t i = 0;
    if (!strcmp(args[i], "-e"))
    {
        e = 1;
        i++;
    }
    else if (!strcmp(args[i], "-n"))
    {
        n = 1;
        i++;
    }
    if (len == 1 && ( e || n ))
    {
        if (!n)
            printf("%c", '\n');
        return 0;
    }
    while (i < len)
    {
        print_it(args[i], e);
        i++;   
    }
    if (!n)
        printf("%c", '\n');
    return 0;
}