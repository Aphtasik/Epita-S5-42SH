#include "lexer.h"

#include <stdio.h>

    /** 0 TOKEN_IF, */
    /** 1 TOKEN_THEN, */
    /** 2 TOKEN_ELIF, */
    /** 3 TOKEN_ELSE, */
    /** 4 TOKEN_FI, */
    /** 5 TOKEN_SEMICOL, */
    /** 6 TOKEN_BACKN, */
    /** 7 TOKEN_SQUOTE, */
    /** 8 TOKEN_WORD, */
    /** 9 TOKEN_EOF, */
    /** 10 TOKEN_ERROR, */
    /** 11 TOKEN_UNKNOWN */

int main(void)
{
    const char *test = "if tamere | then\n pute;";
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
