#include "parse.h"

enum parser_status parse_all(struct lexer *lexer, struct ast *res)
{
    enum parser_status p_stat;
    if ((p_stat = parse_rule_if(lexer, &res)) != PARSER_OK)
        return p_stat;

    if ((p_stat = parse_cmd(lexer, &res)) != PARSER_OK)
        return p_stat;

    return PARSER_OK;
}

struct ast *parse(struct lexer *lexer)
{
    struct ast_root *a_root = init_ast_root();
    struct token *tok = lexer_peek(lexer);

    while (tok->type != TOKEN_EOF)
    {
        a_root->children = realloc(
            a_root->children, sizeof(struct ast *) * (a_root->nb_children + 1));

        enum parser_status p_stat =
            parse_all(lexer, a_root->children[a_root->nb_children++]);
        if (p_stat != PARSER_OK)
        {
            ast_free((struct ast *)a_root);
            return NULL;
        }
    }
    lexer_token_free(lexer);
    return (struct ast *)a_root;
}

int main(void)
{
    char *test = malloc(sizeof(char) * 2);
    test[0] = 'i';
    test[1] = '\0';
    struct lexer *lexer = lexer_new(test);
    struct ast *ast = parse(lexer);
    ast_free(ast);
    lexer_free(lexer);
    return 0;
}

