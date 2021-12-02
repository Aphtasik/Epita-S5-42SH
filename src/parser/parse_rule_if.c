#include "parse.h"

// skip every newline and semicolons found
void skip_newline(struct lexer *lexer)
{
    enum token_type type = (lexer_peek(lexer))->type;
    while (type == TOKEN_BACKN || type == TOKEN_SEMICOL)
        token_free(lexer_pop(lexer));
}

// function that handle every error
enum parser_status handle_parse_error(enum parser_status status,
                                             struct ast **res)
{
    if(status == PARSER_UNEXPECTED_TOKEN)
        warnx("unexpected token");
    if(status == PARSER_NO_COMMAND)
        warnx("no command found");
    ast_free(*res);
    *res = NULL;
    return status;
}

// fill the command struct node
enum parser_status parse_fill_cmd(struct lexer *lexer, struct ast_cmd *cmd)
{
    struct token *tok;
    // in order to check if there is a command
    int i = 0;
    while ((tok = lexer_peek(lexer))->type == TOKEN_WORD)
    {
        // alloc an arg
        cmd->args = realloc(cmd->args, sizeof(char *) * (cmd->nb_args + 1));
        // set it to the value found
        cmd->args[cmd->nb_args++] = strdup(tok->value);
        // go to next token
        token_free(lexer_pop(lexer));
        
        i++;
    }
    if(i == 0)
        return PARSER_NO_COMMAND;
    // unrecognized token so just return ok
    return PARSER_OK;
}

// create a ast_cmd node and fill it if interrested
// return PARSER_OK otherwise
enum parser_status parse_cmd(struct lexer *lexer, struct ast **res)
{
    struct token *tok = lexer_peek(lexer);

    // Not interrested by this token
    if (tok->type != TOKEN_WORD)
        return PARSER_OK;

    //fill cmd node
    struct ast_cmd *a_cmd = init_ast_cmd();
    enum parser_status p_stat = parse_fill_cmd(lexer, a_cmd);

    // place cmd node as child of his parent
    *res = (struct ast *)a_cmd;
    return p_stat;
}

// fill the pointer to array res with all commands node found
// and discard useless tokens
enum parser_status parse_compound_list(struct lexer *lexer, struct ast ***res,
                                       size_t *len)
{
    skip_newline(lexer);
    struct token *tok = lexer_peek(lexer);
    enum parser_status p_stat;
    while (tok->type == TOKEN_WORD || tok->type == TOKEN_BACKN
           || tok->type == TOKEN_SEMICOL)
    {
        skip_newline(lexer);
        // fill the command node, error if no command
        struct ast_cmd *cmd = init_ast_cmd();
        p_stat = parse_fill_cmd(lexer, cmd);
        push_arr(res, len, (struct ast *)cmd);

        // no need to call handle_error cuz handled in parse_rule_if
        if (p_stat != PARSER_OK)
            return p_stat;
    }
    return PARSER_OK;
}

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
    // TODO: handle elif statement
    return PARSER_OK;
}

enum parser_status parse_rule_if(struct lexer *lexer, struct ast **res)
{
    // check if the first token interrest us
    // return PARSER_OK otherwise
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_IF)
        return PARSER_OK;

    // init if node
    struct ast_if *a_if = init_ast_if();
    enum parser_status p_stat;
    // discard if token
    token_free(lexer_pop(lexer));

    // Parse conditions
    p_stat =
        parse_compound_list(lexer, &a_if->conditions, &a_if->nb_conditions);
    if (p_stat != PARSER_OK)
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
    if (p_stat != PARSER_OK)
        return handle_parse_error(p_stat, res);

    // Parse Else clause
    p_stat = parse_else_clause(lexer, a_if);
    if (p_stat != PARSER_OK)
        return handle_parse_error(p_stat, res);

    // Check if "fi"
    tok = lexer_pop(lexer);
    if (tok->type != TOKEN_FI)
    {
        token_free(tok);
        return handle_parse_error(PARSER_MISSING_TOKEN, res);
    }
    token_free(tok);
    // place if node as child of his parent
    *res = (struct ast*) a_if;
    return PARSER_OK;
}

