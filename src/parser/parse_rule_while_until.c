#include "parse.h"

enum parser_status parse_rule_while(struct lexer *lexer, struct ast **res)
{
    // check if the first token interrest us
    // return PARSER_OK otherwise
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_WHILE)
        return PARSER_OK;

    // init while node
    struct ast_while *a_while = init_ast_while();

    // place if node as child of his parent
    *res = (struct ast *)a_while;

    enum parser_status p_stat;
    // discard while token
    token_free(lexer_pop(lexer));

    // Parse conditions
    p_stat = parse_compound_list(lexer, &a_while->conditions,
                                 &a_while->nb_conditions);
    if (p_stat != PARSER_OK)
        return p_stat;

    // Check if "do"
    tok = lexer_pop(lexer);
    if (tok->type != TOKEN_DO)
    {
        token_free(tok);
        return PARSER_MISSING_TOKEN;
    }
    token_free(tok);

    // Parse body
    p_stat = parse_compound_list(lexer, &a_while->body, &a_while->nb_body);
    if (p_stat != PARSER_OK)
        return p_stat;

    // Check if there is a "done"
    tok = lexer_pop(lexer);
    if (tok->type != TOKEN_DONE)
    {
        token_free(tok);
        return PARSER_MISSING_TOKEN;
    }
    token_free(tok);
    return PARSER_OK;
}

enum parser_status parse_rule_until(struct lexer *lexer, struct ast **res)
{
    // check if the first token interrest us
    // return PARSER_OK otherwise
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_UNTIL)
        return PARSER_OK;

    // init while node
    struct ast_until *a_until = init_ast_until();

    // place if node as child of his parent
    *res = (struct ast *)a_until;

    enum parser_status p_stat;
    // discard while token
    token_free(lexer_pop(lexer));

    // Parse conditions
    p_stat = parse_compound_list(lexer, &a_until->conditions,
                                 &a_until->nb_conditions);
    if (p_stat != PARSER_OK)
        return p_stat;

    // Check if "do"
    tok = lexer_pop(lexer);
    if (tok->type != TOKEN_DO)
    {
        token_free(tok);
        return PARSER_MISSING_TOKEN;
    }
    token_free(tok);

    // Parse body
    p_stat = parse_compound_list(lexer, &a_until->body, &a_until->nb_body);
    if (p_stat != PARSER_OK)
        return p_stat;

    // Check if there is a "done"
    tok = lexer_pop(lexer);
    if (tok->type != TOKEN_DONE)
    {
        token_free(tok);
        return PARSER_MISSING_TOKEN;
    }
    token_free(tok);
    return PARSER_OK;
}
