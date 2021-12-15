#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>

enum ast_type
{
    AST_ROOT,
    AST_IF,
    AST_CMD,
    AST_WHILE,
    AST_UNTIL
};

struct ast
{
    enum ast_type type;
};

struct ast_root
{
    struct ast base;

    struct ast **children;
    size_t nb_children;
};

struct ast_if
{
    struct ast base;

    struct ast **conditions;
    size_t nb_conditions;

    struct ast **on_true;
    size_t nb_on_true;

    struct ast **on_false;
    size_t nb_on_false;
};

struct ast_cmd
{
    struct ast base;

    char **args;
    size_t nb_args;
};

struct ast_while
{
    struct ast base;

    struct ast **conditions;
    size_t nb_conditions;

    struct ast **body;
    size_t nb_body;
};

struct ast_until
{
    struct ast base;

    struct ast **conditions;
    size_t nb_conditions;

    struct ast **body;
    size_t nb_body;
};

struct ast_root *init_ast_root(void);
struct ast_if *init_ast_if(void);
struct ast_cmd *init_ast_cmd(void);
struct ast_while *init_ast_while(void);
struct ast_until *init_ast_until(void);
struct ast *init_ast(enum ast_type t);
void ast_free(struct ast *ast);
void push_arr(struct ast ***arr, size_t *len, struct ast *elt);

#endif
