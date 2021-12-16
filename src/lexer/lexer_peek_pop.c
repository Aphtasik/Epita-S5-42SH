#include <stdio.h>

#include "lexer.h"

// return curr token
struct token *lexer_peek(struct lexer *lexer)
{
    if (!lexer->current_tok)
        lexer_fill_current_tok(lexer);

    return lexer->current_tok;
}

// incr pos and return token
struct token *lexer_pop(struct lexer *lexer)
{
    if (lexer->current_tok)
    {
        token_free(lexer->current_tok);
        lexer->current_tok = NULL;
    }

    // if it's the end of string, only return
    if (!lexer->input || *(lexer->input) == '\0')
    {
        struct token *t = token_new(TOKEN_EOF);
        return t;
    }

    char *next_pos = lexer_fill_current_tok(lexer);
    lexer->input = next_pos;

    // Create the token that will be returned
    struct token *t = token_new(lexer->current_tok->type);
    if (lexer->current_tok->value)
    {
        t->value = calloc(strlen(lexer->current_tok->value) + 1, sizeof(char));
        strcpy(t->value, lexer->current_tok->value);
    }

    // Delete the token in the lexer
    if (lexer->current_tok)
        token_free(lexer->current_tok);
    lexer->current_tok = NULL;

    return t;
}
