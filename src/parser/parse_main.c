#include "parse.h"

int main(void)
{
    char *test = strdup("if true; then\n\techo false\nfi");

    struct lexer *lexer = lexer_new(test);
    struct ast *ast = parse(lexer);
    ast_free(ast);
    lexer_free(lexer);
    free(test);
    return 0;
}
