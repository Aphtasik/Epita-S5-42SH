#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>

enum ast_type
{
    AST_ROOT,
    AST_IF,
    AST_CMD
};

struct ast
{
    enum ast_type type;
};

struct ast_root
{
    struct ast base;
    size_t nb_children;
    struct ast **children;
};

struct ast_if
{
    struct ast base;

    struct ast **conditions;
    size_t nb_conditions;

    struct ast **body;
    size_t nb_body;

    struct ast *on_false;
};

struct ast_cmd
{
    struct ast base;
    char **commands;
    size_t nb_commands;
};

#endif
