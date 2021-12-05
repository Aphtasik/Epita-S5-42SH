#include "../parser/parse.h"
#include "exec.h"

#include <stdio.h>

int main(void)
{
    char *test = strdup("if true; then echo false fi");
    //parsing
    struct lexer *lexer = lexer_new(test);
    struct ast *ast = parse(lexer);
    //execution
    eval_ast(ast);
    //free
    ast_free(ast);
    lexer_free(lexer);
    free(test);
    return 0;
}
