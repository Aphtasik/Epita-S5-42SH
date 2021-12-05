#include "parse.h"

// fill the 'else node' of the ast_if node
enum parser_status parse_else_clause(struct lexer *lexer, struct ast_if *a_if)
{
    struct token *tok = lexer_peek(lexer);

    if (tok->type != TOKEN_ELSE || tok->type != TOKEN_ELIF)
        return PARSER_OK;

    enum parser_status p_stat;
    if (tok->type == TOKEN_ELSE)
    {
        // discard else token
        token_free(lexer_pop(lexer));
        p_stat =
            parse_compound_list(lexer, &(a_if->on_false), &(a_if->nb_on_false));
        if (p_stat != PARSER_OK)
            return p_stat;
        return PARSER_OK;
    }
    // elif statement
    else
    {
        // alloc the place needed to attach the if node
        a_if->on_false = realloc(a_if->on_false, sizeof(struct ast *));
        a_if->nb_on_false = 1;
        // function will handle discarding token
        p_stat = parse_rule_if(lexer, &(a_if->on_false[0]));
        if (p_stat != PARSER_OK)
            return p_stat;
    }
    return PARSER_OK;
}

enum parser_status parse_rule_if(struct lexer *lexer, struct ast **res)
{
    // check if the first token interrest us
    // return PARSER_OK otherwise
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_IF && tok->type != TOKEN_ELIF)
        return PARSER_OK;

    // check if it is an elif statement
    int is_elif = 0;
    if (tok->type == TOKEN_ELIF)
        is_elif = 1;

    // init if node
    struct ast_if *a_if = init_ast_if();

    // place if node as child of his parent
    *res = (struct ast *)a_if;

    enum parser_status p_stat;
    // discard if token
    token_free(lexer_pop(lexer));

    // Parse conditions
    p_stat =
        parse_compound_list(lexer, &a_if->conditions, &a_if->nb_conditions);
    if (p_stat != PARSER_OK)
        return p_stat;

    // Check if "then"
    tok = lexer_pop(lexer);
    if (tok->type != TOKEN_THEN)
    {
        token_free(tok);
        return PARSER_MISSING_TOKEN;
    }
    token_free(tok);

    // Parse on_true
    p_stat = parse_compound_list(lexer, &a_if->on_true, &a_if->nb_on_true);
    if (p_stat != PARSER_OK)
        return p_stat;

    // Parse Else clause
    p_stat = parse_else_clause(lexer, a_if);
    if (p_stat != PARSER_OK)
        return p_stat;

    // Check if thre is a "fi" if we're not in elif statement
    if (!is_elif)
    {
        tok = lexer_pop(lexer);
        if (tok->type != TOKEN_FI)
        {
            token_free(tok);
            return PARSER_MISSING_TOKEN;
        }
        token_free(tok);
    }
    return PARSER_OK;
}
