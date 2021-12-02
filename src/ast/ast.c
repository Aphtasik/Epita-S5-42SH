#include "ast.h"

// ###### INIT FUNCTIONS

struct ast_root *init_ast_root(void)
{
    struct ast_root *new = malloc(sizeof(struct ast_root));
    new->nb_children = 0;
    new->children = NULL;
    new->base.type = AST_ROOT;
    return new;
}

struct ast_if *init_ast_if(void)
{
    struct ast_if *new = malloc(sizeof(struct ast_if));

    new->nb_conditions = 0;
    new->conditions = NULL;

    new->nb_on_true = 0;
    new->on_true = NULL;

    new->on_false = NULL;
    new->nb_on_false = 0;

    new->base.type = AST_IF;
    return new;
}

struct ast_cmd *init_ast_cmd(void)
{
    struct ast_cmd *new = malloc(sizeof(struct ast_cmd));
    new->commands = NULL;
    new->nb_commands = 0;
    new->base.type = AST_CMD;
    return new;
}

struct ast *init_ast(enum ast_type t)
{
    switch (t)
    {
    case AST_ROOT:
        return (struct ast *)init_ast_root();
    case AST_IF:
        return (struct ast *)init_ast_if();
    case AST_CMD:
        return (struct ast *)init_ast_cmd();
    default:
        return NULL;
    }
}
// ##### UTILITY FUNCTIONS

void push_arr(struct ast** arr, size_t *len, struct ast *elt)
{
    arr = realloc(arr, sizeof(struct ast*) * ((*len) + 1));
    arr[(*len)++] = elt;
}

// ##### FREE FUNCTIONS

void ast_free(struct ast *ast)
{
    if (ast == NULL)
        return;

    enum ast_type t = ast->type;

    if (t == AST_ROOT)
    {
        struct ast_root *a_root = (struct ast_root *)ast;
        for (size_t i = 0; i < a_root->nb_children; i++)
            ast_free(a_root->children[i]);
        free(a_root->children);
    }
    else if (t == AST_IF)
    {
        struct ast_if *a_if = (struct ast_if *)ast;
        for (size_t i = 0; i < a_if->nb_conditions; i++)
            ast_free(a_if->conditions[i]);
        free(a_if->conditions);
        for (size_t i = 0; i < a_if->nb_on_true; i++)
            ast_free(a_if->on_true[i]);
        free(a_if->on_true);
        for (size_t i = 0; i < a_if->nb_on_false; i++)
            ast_free(a_if->on_false[i]);
    }
    else
    {
        struct ast_cmd *a_cmd = (struct ast_cmd *)ast;
        for (size_t i = 0; i < a_cmd->nb_commands; i++)
            free(a_cmd->commands[i]);
        free(a_cmd->commands);
    }
    free(ast);
}

int main(void)
{
    struct ast_if *a_if = init_ast_if();
    struct ast_cmd *a_cmd= init_ast_cmd();
    push_arr(a_if->conditions, &a_if->nb_conditions, (struct ast*)a_cmd);
    ast_free((struct ast*)a_if);
}
