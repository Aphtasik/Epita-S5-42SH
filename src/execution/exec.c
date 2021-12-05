#include "exec.h"

#define NB_BUILTINS 3

int true_it(char **args, size_t nbargs)
{
    if (nbargs && args[0][0])
        return 0;
    return 0;
}

int false_it(char **args, size_t nbargs)
{
    if (nbargs && args[0][0])
        return 1;
    return 1;
}

// TODO: add builtins when implemented
struct builtins_fun builtins[] = {
    //{"cd", (*cd)},
    { "echo", (*echo_it) },
    { "true", (*true_it) },
    { "false", (*false_it) }
};

int is_builtin(char *cmd)
{
    for (int i = 0; i < 2; i++)
    {
        if (!strcmp(cmd, builtins[i].name))
            return 1;
    }
    return 0;
}

int exec_fork(char **cmd, size_t nbargs)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        warnx("fork error");
        // operation canceled error code
        return ECANCELED;
    }
    if (!pid)
    {
        // children
        char **format = malloc(sizeof(char *) * nbargs + 1);
        format[0] = NULL;
        for (size_t i = 0; i < nbargs; i++)
            format[i + 1] = cmd[i];
        execvp(format[1], format);
        if (errno == 127)
            warnx("%s: command not found\n", cmd[0]);
        return errno;
    }
    // in parent
    int status;
    waitpid(pid, &status, 0);
    return 0;
}

int exec_builtins(char **cmd, size_t nbargs)
{
    for (int i = 0; i < NB_BUILTINS; i++)
    {
        if (!strcmp(cmd[0], builtins[i].name))
            return builtins[i].fun(cmd, nbargs);
    }
    // maybe useless because called only if is_builtin is true
    warnx("%s: command not found\n", cmd[0]);
    return 127;
}

void exec_rule_if(struct ast *ast)
{
    struct ast_if *a_if = (struct ast_if *)ast;
    int ret = 0;
    for (size_t i = 0; i < a_if->nb_conditions; i++)
        ret = eval_ast(a_if->conditions[i]);
    // if last condition commmand return correctly, execute then clause
    if (!ret)
    {
        for (size_t i = 0; i < a_if->nb_on_true; i++)
            eval_ast(a_if->on_true[i]);
    }
    // else execute the else clause
    else
    {
        // NULL else case handled by the nb_conditions equal to 0
        for (size_t i = 0; i < a_if->nb_on_false; i++)
            eval_ast(a_if->conditions[i]);
    }
}

int eval_ast(struct ast *ast)
{
    // if the parser exited with an error
    if (!ast)
        return 1;
    int ret = 0;
    switch (ast->type)
    {
    case AST_IF:
        exec_rule_if(ast);
        break;
    case AST_CMD:
        if (is_builtin(((struct ast_cmd *)ast)->args[0]))
        {
            ret = exec_builtins(((struct ast_cmd *)ast)->args,
                                ((struct ast_cmd *)ast)->nb_args);
            if (ret != 0)
                warnx("command %s exited with error code %d\n",
                      ((struct ast_cmd *)ast)->args[0], ret);
            return ret;
        }
        else
        {
            ret = exec_fork(((struct ast_cmd *)ast)->args,
                            ((struct ast_cmd *)ast)->nb_args);
            if (ret != 0)
                warnx("command %s exited with error code %d\n",
                      ((struct ast_cmd *)ast)->args[0], ret);
            return ret;
        }
        break;
    case AST_ROOT:
        for (size_t i = 0; i < ((struct ast_root *)ast)->nb_children; i++)
            eval_ast(((struct ast_root *)ast)->children[i]);
        break;
    }
    return 0;
}
