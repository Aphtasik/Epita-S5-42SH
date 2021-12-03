#include "lexer.h"

struct lexer *lexer_new(char *input)
{
    struct lexer *new = malloc(sizeof(struct lexer));
    if (!new)
        return NULL;

    new->input = input;
    new->current_tok = NULL;

    return new;
}

void lexer_free(struct lexer *lexer)
{
    if (lexer)
    {
        if (lexer->current_tok)
            token_free(lexer->current_tok);

        if (lexer->input)
            free(lexer->input);

        free(lexer);
        lexer = NULL;
    }
}
