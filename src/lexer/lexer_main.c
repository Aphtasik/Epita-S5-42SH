#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

/** 0 TOKEN_IF, */
/** 1 TOKEN_THEN, */
/** 2 TOKEN_ELIF, */
/** 3 TOKEN_ELSE, */
/** 4 TOKEN_FI, */
/** 5 TOKEN_SEMICOL, */
/** 6 TOKEN_BACKN, */
/** 7 TOKEN_PIPE, */
/** 8 TOKEN_SQUOTE, */
/** 9 TOKEN_WORD, */
/** 10 TOKEN_EOF, */
/** 11 TOKEN_ERROR, */
/** 12 TOKEN_UNKNOWN */

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
