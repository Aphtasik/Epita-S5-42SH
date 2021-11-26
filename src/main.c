#include "lexer.h"

#include <stdio.h>

int main(void)
{
    const char *test = "if tamere; then pute";
    struct lexer *l = lexer_new(test);
    struct token *t;
    enum token_type type = TOKEN_IF;

    while(type != TOKEN_EOF)
    {
         t = lexer_peek(l);
         printf("%d\n", t->type);
         t = lexer_pop(l);
         type = t->type;
         token_free(t);
    }

    lexer_free(l);

    return 0;
}
