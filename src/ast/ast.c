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

    new->nb_body = 0;
    new->body = NULL;

    new->on_false = NULL;

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
        for (size_t i = 0; i < a_if->nb_body; i++)
            ast_free(a_if->body[i]);
        free(a_if->body);

        ast_free(a_if->on_false);
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
