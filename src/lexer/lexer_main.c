#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

int main(void)
{
    char *test = "if while for do done";
    struct lexer *l = lexer_new(test);
    struct token *t;
    enum token_type type = TOKEN_IF;

    while (type != TOKEN_EOF)
    {
        t = lexer_peek(l);
        printf("%s, %d\n", t->value, t->type);
        t = lexer_pop(l);
        type = t->type;
        token_free(t);
    }
    t = lexer_pop(l);
    type = t->type;
    token_free(t);

    l->input = NULL;
    lexer_free(l);

    return 0;
}
