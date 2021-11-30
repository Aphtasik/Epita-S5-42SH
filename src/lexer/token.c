#include "token.h"

#include "alloc.h"

// Struct that allows us to avoid a huge switch
static struct token token_pairs[] = 
{
    {TOKEN_IF, "if"},
    {TOKEN_THEN, "then"},
    {TOKEN_ELIF, "elif"},
    {TOKEN_ELSE, "else"},
    {TOKEN_FI, "fi"},
    {TOKEN_SEMICOL, ";"},
    {TOKEN_PIPE, "|"},
    {TOKEN_BACKN, "\n"},
    {TOKEN_EOF, "\0"},
    {TOKEN_UNKNOWN, NULL},
};

struct token *token_new(enum token_type type)
{
    struct token *new = zalloc(sizeof(struct token));
     new->type = type;
     new->value = NULL;
    return new;
}

void token_free(struct token *token)
{
    if (token)
    {
        if (token->value)
            free(token->value);

        free(token);
    }
}

enum token_type token_get_type(char *token_str)
{
    // Iterate throught the token_pairs struct
    // Compare string and get the token type
    for (size_t i = 0; token_pairs[i].value; i++)
    {
        if (!strcmp(token_pairs[i].value, token_str))
            return token_pairs[i].type;
    }
    return TOKEN_WORD;
}
