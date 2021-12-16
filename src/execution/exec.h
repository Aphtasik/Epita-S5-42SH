#ifndef EXEC_H
#define EXEC_H

// TODO: builtins.h
#include <err.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../ast/ast.h"
#include "../builtins/builtins.h"

struct builtins_fun
{
    char *name;
    int (*fun)(char **args, size_t nbargs);
};

int is_builtin(char *cmd);
int exec_fork(char **cmd, size_t nb_args);
int exec_builtins(char **cmd, size_t nbargs);

int eval_ast(struct ast *ast);

#endif
