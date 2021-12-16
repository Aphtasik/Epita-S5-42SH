#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int cd(char **args, size_t len, )
{
    if (len == 2 && strcmp(args[1], "-")
    {
        printf("%s", getenv("PWD"));
        return 0;
    }
    char *path = malloc(sizeof(char));
    if (!path)
        errx(1, "malloc of path failed in cd.");
    size_t len_dir = 0;
    if (len == 1)
    {
        char *path_home = getenv("HOME");
        if (!path_home)
            return 0;
        else
        {
            len_dir = strlen(path_home);
            path = realloc(path, len_dir + 1);
            if (!path)
                errx(1, "realloc of path failed in cd.");
            strcpy(path, path_home);
        }
    }
    else
    {
        len_dir = strlen(args[1]);
        path = realloc(path, len_dir + 1);
        if (!path)
            errx(1, "realloc of path failed in cd.");
        strcpy(path, args[1]);
        char *pwd = getenv("PWD");
        len_dir = strlen(pwd);
        char *old_pwd = malloc(sizeof(char) * (len_dir + 1));
        if (!old_pwd)
            errx(1, "old_pwd malloc failed in malloc.");
        old_pwd = strcpy(old_pwd, pwd);
        if (pwd[len_dir - 1] != '/')
        {
            strcat(pwd, "/");
        }
        strcat(pwd, path);
        path = pwd;
        convert(path);
    }
}
