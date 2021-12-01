#include "parse.h"

void skip_newline(struct lexer *lexer)
{
    struct token *tok = NULL;

    while ((lexer_peek(lexer))->type == TOKEN_BACKN)
        token_free(lexer_pop(lexer));
}

static enum parser_status handle_parse_error(enum parser_status status,
                                             struct ast **res)
{
    warnx("unexpected token");
    ast_free(*res);
    *res = NULL;
    return status;
}

struct ast **parse_compound_list(struct lexer *lexer)
{
    struct token *tok = NULL;

}

enum parser_status parse_rule_if(struct lexer *lexer, struct ast **res)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_IF)
        return PARSER_OK;

    struct ast_if *a_if = init_ast_if();
    enum parser_status p_stat;
    token_free(lexer_pop(lexer));

    // Parse conditions
    parse_compound_list(lexer);

    // Check if "then"
    tok = lexer_pop(lexer);
    if (tok->type != TOKEN_THEN) 
    {
        token_free(tok);
        return handle_parse_error(PARSER_UNEXPECTED_TOKEN, res);
    }
    token_free(tok);

    // Parse on_true
    parse_compound_list(lexer);

    // Parse Else clause
    parse_else_clause(lexer);

    // Check if "fi"
    tok = lexer_pop(lexer);
    if (tok->type != TOKEN_FI) 
    {
        token_free(tok);
        return handle_parse_error(PARSER_UNEXPECTED_TOKEN, res);
    }
    token_free(tok);

    // FOutre a_if ds res TODO;
    return PARSER_OK;
}


enum parser_status parse(struct ast **res, struct lexer *lexer)
{
    // If we're at the end of file, there's no input
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_EOF)
    {
        *res = NULL;
        return PARSER_OK;
    }

    // try to parse an expression. if an error occured, free the
    // produced ast and return the same error code
    enum parser_status status = parse_exp(res, lexer);
    if (status != PARSER_OK)
        return handle_parse_error(status, res);

    // once parsing the expression is done, we should have
    // reached the end of file.
    if (lexer_peek(lexer)->type == TOKEN_EOF)
        return PARSER_OK;

    // if we didn't reach the end of file, it's an error
    return handle_parse_error(PARSER_UNEXPECTED_TOKEN, res);
}
