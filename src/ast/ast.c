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

struct ast_while *init_ast_while(void)
{
    struct ast_while *new = malloc(sizeof(struct ast_while));

    new->nb_conditions = 0;
    new->conditions = NULL;

    new->nb_body = 0;
    new->body = NULL;

    new->base.type = AST_WHILE;
    return new;
}

struct ast_until *init_ast_until(void)
{
    struct ast_until *new = malloc(sizeof(struct ast_until));

    new->nb_conditions = 0;
    new->conditions = NULL;

    new->nb_body = 0;
    new->body = NULL;

    new->base.type = AST_UNTIL;
    return new;
}

struct ast_cmd *init_ast_cmd(void)
{
    struct ast_cmd *new = malloc(sizeof(struct ast_cmd));
    new->args = NULL;
    new->nb_args = 0;
    new->base.type = AST_CMD;
    return new;
}

struct ast_op *init_ast_op(enum ast_type type)
{
    struct ast_op *new = malloc(sizeof(struct ast_op));
    new->base.type = type;
    new->value = NULL;
    new->leftc = NULL;
    new->rightc = NULL;
    return new;
}

struct ast_not *init_ast_not()
{
    struct ast_not *new = malloc(sizeof(struct ast_not));
    new->base.type = AST_NOT;
    new->value = NULL;
    return new;
}

struct ast_word *init_ast_word()
{
    struct ast_word *new = malloc(sizeof(struct ast_word));
    new->base.type = AST_WORD;
    new->value = NULL;
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
    case AST_REDIR:
        return (struct ast *)init_ast_op(AST_REDIR);
    case AST_AND:
        return (struct ast *)init_ast_op(AST_AND);
    case AST_OR:
        return (struct ast *)init_ast_op(AST_OR);
    case AST_NOT:
        return (struct ast *)init_ast_not();
    case AST_WORD:
        return (struct ast *)init_ast_word();
    case AST_WHILE:
        return (struct ast *)init_ast_while();
    case AST_UNTIL:
        return (struct ast *)init_ast_until();
    default:
        warnx("init_ast: unrecognized type of ast");
        return NULL;
    }
}
// ##### UTILITY FUNCTIONS

void push_arr(struct ast ***arr, size_t *len, struct ast *elt)
{
    *arr = realloc(*arr, sizeof(struct ast *) * ((*len) + 1));
    (*arr)[(*len)++] = elt;
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
        if (a_if->nb_on_false)
        {
            for (size_t i = 0; i < a_if->nb_on_false; i++)
                ast_free(a_if->on_false[i]);
            free(a_if->on_false);
        }
    }
    else if (t == AST_CMD)
    {
        struct ast_cmd *a_cmd = (struct ast_cmd *)ast;
        for (size_t i = 0; i < a_cmd->nb_args; i++)
            free(a_cmd->args[i]);
        free(a_cmd->args);
    }
<<<<<<< HEAD
    else if (t == AST_NOT)
    {
        struct ast_not *a_not = (struct ast_not *)ast;
        ast_free(a_not->value);
    }
    else if (t == AST_WORD)
    {
        struct ast_word *a_word = (struct ast_word *)ast;
        free(a_word->value);
    }
    else // ast_op
    {
        struct ast_op *a_op = (struct ast_op *)ast;
        free(a_op->value);
        ast_free(a_op->leftc);
        ast_free(a_op->rightc);
=======
    else if (t == AST_WHILE)
    {
        struct ast_while *a_while = (struct ast_while *)ast;
        for (size_t i = 0; i < a_while->nb_conditions; i++)
            ast_free(a_while->conditions[i]);
        free(a_while->conditions);
        for (size_t i = 0; i < a_while->nb_body; i++)
            ast_free(a_while->body[i]);
        free(a_while->body);
    }
    else if (t == AST_UNTIL)
    {
        struct ast_until *a_until = (struct ast_until *)ast;
        for (size_t i = 0; i < a_until->nb_conditions; i++)
            ast_free(a_until->conditions[i]);
        free(a_until->conditions);
        for (size_t i = 0; i < a_until->nb_body; i++)
            ast_free(a_until->body[i]);
        free(a_until->body);
>>>>>>> master
    }
    free(ast);
}
