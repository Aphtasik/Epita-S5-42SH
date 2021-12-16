#include "exec.h"

#define NB_BUILTINS 4

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
    { "false", (*false_it) },
    { "exit", (*exit_it) }
};

int is_builtin(char *cmd)
{
    for (int i = 0; i < NB_BUILTINS; i++)
    {
        if (!strcmp(cmd, builtins[i].name))
            return 1;
    }
    return 0;
}

int exec_fork(char **cmd, size_t nb_args)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        printf("fork error\n");
        // operation canceled error code
        exit(ECANCELED);
    }
    if (!pid)
    {
        // children
        cmd = realloc(cmd, (nb_args + 1) * sizeof(char *));
        cmd[nb_args] = NULL;
        execvp(cmd[0], cmd);
        exit(errno);
    }
    // in parent
    int status;
    int ret = waitpid(pid, &status, 0);
    if (ret == -1)
        printf("waitpid error\n");
    ret = WEXITSTATUS(status);
    if (ret == 2)
    {
        printf("%s: command not found\n", cmd[0]);
        return 127;
    }
    else if (ret > 0)
    {
        printf("command return with exit code %d\n", ret);
        return ret;
    }
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
    printf("%s: command not found\n", cmd[0]);
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
            eval_ast(a_if->on_false[i]);
    }
}

void exec_rule_while(struct ast *ast)
{
    struct ast_while *a_while = (struct ast_while *)ast;
    int ret = 0;
    for (size_t i = 0; i < a_while->nb_conditions; i++)
        ret = eval_ast(a_while->conditions[i]);
    // while last condition commmand return correctly, execute do clause
    while (!ret)
    {
        // execute body
        for (size_t i = 0; i < a_while->nb_body; i++)
            eval_ast(a_while->body[i]);

        // re execute all conditions to update ret
        for (size_t i = 0; i < a_while->nb_conditions; i++)
            ret = eval_ast(a_while->conditions[i]);
    }
}

void exec_rule_until(struct ast *ast)
{
    struct ast_until *a_until = (struct ast_until *)ast;
    int ret = 0;
    for (size_t i = 0; i < a_until->nb_conditions; i++)
        ret = eval_ast(a_until->conditions[i]);
    // while last condition commmand return incorrectly, execute do clause
    while (ret)
    {
        // execute body
        for (size_t i = 0; i < a_until->nb_body; i++)
            eval_ast(a_until->body[i]);

        // re execute all conditions to update ret
        for (size_t i = 0; i < a_until->nb_conditions; i++)
            ret = eval_ast(a_until->conditions[i]);
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
    case AST_WHILE:
        exec_rule_while(ast);
        break;
    case AST_UNTIL:
        exec_rule_until(ast);
        break;
    case AST_CMD:
        if (is_builtin(((struct ast_cmd *)ast)->args[0]))
        {
            return exec_builtins(((struct ast_cmd *)ast)->args,
                                 ((struct ast_cmd *)ast)->nb_args);
        }
        else
        {
            return exec_fork(((struct ast_cmd *)ast)->args,
                             ((struct ast_cmd *)ast)->nb_args);
        }
        break;
    case AST_ROOT:
        for (size_t i = 0; i < ((struct ast_root *)ast)->nb_children; i++)
            ret = eval_ast(((struct ast_root *)ast)->children[i]);
        return ret;
    default:
        puts("Not implemented");
        break;
    }
    return 0;
}
