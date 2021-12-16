#ifndef AST_H
#define AST_H

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

enum ast_type
{
    AST_ROOT,
    AST_IF,
    AST_CMD,
    AST_REDIR,
    AST_AND,
    AST_OR,
    AST_NOT,
    AST_WORD,
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

struct ast_op
{
    struct ast base;

    char *value; // can be util in some cases like redir
    struct ast *leftc;
    struct ast *rightc;
};

struct ast_word
{
    struct ast base;

    char *value;
};

struct ast_not
{
    struct ast base;

    struct ast *value;
};

// init
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
struct ast_op *init_ast_op(enum ast_type type);
struct ast_not *init_ast_not();
struct ast_word *init_ast_word();

void ast_free(struct ast *ast);
void push_arr(struct ast ***arr, size_t *len, struct ast *elt);

#endif
