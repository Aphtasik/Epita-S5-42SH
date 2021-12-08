#pragma once

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum token_type
{
    TOKEN_IF = 1,
    TOKEN_THEN,
    TOKEN_ELIF,
    TOKEN_ELSE,
    TOKEN_FI,
    TOKEN_WHILE,
    TOKEN_DO,
    TOKEN_DONE,
    TOKEN_FOR,
    TOKEN_CASE,
    TOKEN_ESAC,
    TOKEN_SEMICOL,
    TOKEN_BACKN,
    TOKEN_REDIR,
    TOKEN_PIPE,
    TOKEN_AMPERS,
    TOKEN_OR,
    TOKEN_AND,
    TOKEN_NOT,
    TOKEN_ARITH,
    TOKEN_VAR,
    TOKEN_QT,
    TOKEN_COMMENT,
    TOKEN_WORD,
    TOKEN_EOF,
    TOKEN_ERROR,
    TOKEN_UNKNOWN
};

struct token
{
    enum token_type type; ///< The kind of token
    char *value; ///< If the token is a number, its value
};

/**
 * \brief Allocate a new token
 */
struct token *token_new(enum token_type type);

/**
 * \brief Frees a token
 */
void token_free(struct token *token);

// Get the token type corresponding to a given string
enum token_type token_get_type(char *token_str);
