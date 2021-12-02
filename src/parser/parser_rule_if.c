#include "parse.h"

void skip_newline(struct lexer *lexer)
{
    struct token *tok = NULL;
    enum token_type type = (lexer_peek(lexer))->type;
    while (type == TOKEN_BACKN || type == TOKEN_SEMICOL)
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

// fill the command struct node
enum parser_status parse_cmd(struct lexer *lexer, struct ast_cmd *cmd)
{
    struct token *tok = lexer_peek(lexer);
    while(tok->type == TOKEN_WORD)
    {
        cmd->args = realloc(cmd->args, sizeof(char *) * (cmd->nb_args + 1));
        cmd->args[cmd->nb_args++] = tok->value;
    }
    return PARSER_OK;
}

// fill the pointer to array res with all commands node found
// and discard useless tokens
enum parser_status parse_compound_list(struct lexer *lexer, struct ast ***res, size_t *len)
{
    skip_newline(lexer);
    struct token *tok = lexer_peek(lexer);
    enum parser_status p_stat;
    while(tok->type == TOKEN_WORD || tok->type == TOKEN_BACKN
            || tok->type == TOKEN_SEMICOL)
    {
        skip_newline(lexer);
        struct ast_cmd *cmd = init_ast_cmd();
        p_stat = parse_cmd(lexer, cmd);
        push_arr(res, len, (struct ast*)cmd);
        if(p_stat != PARSER_OK)
        {
            p_stat = handle_parse_error(p_stat, res);
            return p_stat;
        }
    }
}

// fill the 'else node' of the ast_if node
enum parser_status parse_else_clause(struct lexer *lexer, struct ast_if *a_if)
{
    struct token *tok = lexer_peek(lexer);
    if(  tok->type != TOKEN_ELSE
            || tok->type != TOKEN_ELIF)
        return PARSER_OK;

    enum parser_status p_stat;
    if(tok->type == TOKEN_ELSE)
    {
        token_free(lexer_pop(lexer));
        p_stat = parse_compound_list(lexer, &(a_if->on_false),
                &(a_if->nb_on_false));
        if (p_stat != PARSER_OK)
            return p_stat;
        return PARSER_OK;
    }
    //TODO: handle elif statement 
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
    p_stat = parse_compound_list(lexer, &a_if->conditions,
            &a_if->nb_conditions);
    if(p_stat != PARSER_OK)
        return handle_parse_error(p_stat, res);

    // Check if "then"
    tok = lexer_pop(lexer);
    if (tok->type != TOKEN_THEN) 
    {
        token_free(tok);
        return handle_parse_error(PARSER_UNEXPECTED_TOKEN, res);
    }
    token_free(tok);

    // Parse on_true
    p_stat = parse_compound_list(lexer, &a_if->on_true, &a_if->nb_on_true);
    if(p_stat != PARSER_OK)
        return handle_parse_error(p_stat, res);

    // Parse Else clause
    p_stat = parse_else_clause(lexer, a_if);
    if(p_stat != PARSER_OK)
        return handle_parse_error(p_stat, res);

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

