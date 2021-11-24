#pragma once

#include <unistd.h>

enum token_type
{
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELIF,
    TOKEN_ELSE,
    TOKEN_FI,
    TOKEN_SEMICOL,
    TOKEN_BACKN,
    TOKEN_SQUOTE,
    TOKEN_WORD,
    TOKEN_EOF,
    TOKEN_ERROR,
    TOKEN_UNKNOWN
};

struct token
{
    enum token_type type; ///< The kind of token
    char* value; ///< If the token is a number, its value
};

/**
 * \brief Allocate a new token
 */
struct token *token_new(enum token_type type);

/**
 * \brief Frees a token
 */
void token_free(struct token *token);
