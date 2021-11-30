#include "parser.h"

// functions declaration
enum parser_status parse_if(struct ast **res, struct lexer *lexer);
enum parser_status parse_expr(struct ast **res, struct lexer *lexer);
enum parser_status parse(struct ast **res, struct lexer *lexer);

static enum parser_status handle_error(enum parser_status status,
                        struct ast **res)
{
    if(status == PARSER_NOMATCH)
        warnx("unecpected token");
    else if (status == PARSER_ERROR)
        warnx("parser error");
    else if (status == PARSER_MISSING_OPERAND)
        warnx("missing operand");
    ast_free(*res);
    return status;
}


/**
 * @brief this function parse an if statement
 * @exp: if(.*)fi
 */
enum parser_status parse_if(struct ast **res, struct lexer *lexer)
{
    // get rid of the if token
    struct ast_if *if_node = init_ast_if();
    token_free(lexer_pop(lexer));
    // parse the condition statement
    enum parser_status stat = parse_expr(res, lexer);
    if (stat != PARSER_OK)
        return stat;
    if (lexer_peek(lexer)->type != TOKEN_THEN)
        handle_error(PARSER_MISSING_OPERAND, res);
    token_free(lexer_pop(lexer));
    // parse the then statement
}

enum parser_status parse_then()

/**
 * @brief function that parse an expression
 */
enum parser_status parse_expr(struct ast **res, struct lexer *lexer)
{
    enum parser_status stat = parse_cmd(res, lexer);
    if (stat != PARSER_OK)
        return stat;
    while (1)
    {
        struct token *tok = lexer_peek(lexer);
    }
}

enum parser_status parse(struct ast **res, struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_EOF)
    {
        *res = NULL;
        return PARSER_OK;
    }
        *res = NULL;
    enum parser_status stat = parse_expr(res, lexer);
    if(stat != PARSER_OK)
        handle_error(stat, res);

    if (lexer_peek(lexer)->type == TOKEN_EOF)
        return PARSER_OK;
    return handle_error(PARSER_NOMATCH, res);
}
