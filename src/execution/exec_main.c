#include <stdio.h>

#include "../parser/parse.h"
#include "exec.h"

int main(void)
{
    char *test = strdup("if false; then echo first \nelse echo second \nfi");
    // parsing
    struct lexer *lexer = lexer_new(test);
    struct ast *ast = parse(lexer);
    // execution
    eval_ast(ast);
    // free
    ast_free(ast);
    lexer_free(lexer);
    free(test);
    return 0;
}
