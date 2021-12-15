#include "parse.h"

// function that handle every error
enum parser_status handle_parse_error(enum parser_status status,
                                      struct ast **res)
{
    if (status == PARSER_UNEXPECTED_TOKEN)
        printf("unexpected token\n");
    else if (status == PARSER_NO_COMMAND)
        printf("no command found\n");
    else if (status == PARSER_MISSING_TOKEN)
        printf("missing token\n");
    ast_free(*res);
    *res = NULL;
    return status;
}

enum parser_status parse_all(struct lexer *lexer, struct ast **res)
{
    enum parser_status p_stat;
    // parse if rule, must return ok if not interrested
    if ((p_stat = parse_rule_if(lexer, res)) != PARSER_OK)
        return p_stat;
    // parse every while rule
    if ((p_stat = parse_rule_while(lexer, res)) != PARSER_OK)
        return p_stat;
    // parse every rule until
    if ((p_stat = parse_rule_until(lexer, res)) != PARSER_OK)
        return p_stat;
    // parse every word found
    if ((p_stat = parse_cmd(lexer, res)) != PARSER_OK)
        return p_stat;

    return PARSER_OK;
}

struct ast *parse(struct lexer *lexer)
{
    // init root node
    struct ast_root *a_root = init_ast_root();
    struct token *tok = lexer_peek(lexer);

    while (tok->type != TOKEN_EOF)
    {
        // alloc a new children to thee root node
        a_root->children = realloc(
            a_root->children, sizeof(struct ast *) * (a_root->nb_children + 1));

        // pass the new children address to the general parse function
        enum parser_status p_stat =
            parse_all(lexer, &a_root->children[a_root->nb_children++]);
        token_free(lexer_pop(lexer));
        // handling error
        if (p_stat != PARSER_OK)
        {
            struct ast *root = (struct ast *)a_root;
            handle_parse_error(p_stat, &root);
            return NULL;
        }
        tok = lexer_peek(lexer);
    }
    return (struct ast *)a_root;
}
