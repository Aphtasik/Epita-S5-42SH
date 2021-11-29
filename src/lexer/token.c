#include "token.h"

#include <err.h>
#include <stdlib.h>

#include "alloc.h"

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
